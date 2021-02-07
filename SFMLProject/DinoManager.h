#pragma once
#include "Dino.h"
#include "Thor/Resources.hpp"
#include "pthread.h"
#include "ThreadStructs.h"
#include "Config.h"

class DinoManager {
private:
	thor::ResourceHolder<sf::Texture, std::string> _holder;
	pthread_mutex_t _buf_mutex;

	std::vector<Dino*> _dinos;

	const std::string _dinoNames[4] = { "doux", "mort", "tard", "vita" };

public:

	void Update(float dTime);

	void Draw(float deltaTime, sf::RenderWindow& window);

	void InitialiseDinos(int amountOfDinos);

	std::vector<Dino*> GetDinos() { return _dinos; };

private:

	pthread_t dinoUpdateThreads[NUM_THREADS];

	void DinoBuilder(int amount, int iteration = 0);

	static void* DinoBuilderThread(void* paramsPtr) {
		dinoinit_args params = *(dinoinit_args*)paramsPtr;

		params.dinoManager->DinoBuilder(params.amount, params.iteration);

		return nullptr;
	}

	std::vector<Dino*> _dinosToBeUpdated;
	std::vector<Dino*> _dinosFinishedUpdating;

	void UpdateThreadedPool();

	std::vector<std::vector<Dino*>> _groups;
	std::vector<std::vector<Dino*>> _groupsFinished;

	pthread_mutex_t _dinosToBeUpdated_mutex;
	pthread_mutex_t _dinosUpdated_mutex;

	float _currentDTime = 0;

	static void* DinoUpdateThread(void* params) {

		DinoManager* dinoManager = (DinoManager*)params;

		dinoManager->UpdateThreadedPool();

		return nullptr;
	}

};