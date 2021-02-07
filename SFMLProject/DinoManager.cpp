#include "DinoManager.h"
#include <algorithm>
#include <random>
#include <functional>
#include <iostream>

void DinoManager::InitialiseDinos(int amountOfDinos) {

	sf::Clock clock;

	_holder.acquire("doux", thor::Resources::fromFile<sf::Texture>("doux.png"));
	_holder.acquire("mort", thor::Resources::fromFile<sf::Texture>("mort.png"));
	_holder.acquire("tard", thor::Resources::fromFile<sf::Texture>("tard.png"));
	_holder.acquire("vita", thor::Resources::fromFile<sf::Texture>("vita.png"));

	int amount = amountOfDinos / NUM_THREADS;

	if (IS_THREADED) {
		pthread_mutex_init(&_buf_mutex, NULL);

		pthread_t threads[NUM_THREADS];
		for (int i = 0; i < NUM_THREADS; ++i) {
			pthread_create(&threads[i], NULL, &DinoManager::DinoBuilderThread, new dinoinit_args(this, amount, i));
		}

		for (int i = 0; i < NUM_THREADS; ++i) {
			pthread_join(threads[i], NULL);
		}

		std::cout << "Time taken to initialise Dinos (threaded): " << clock.getElapsedTime().asMilliseconds() << "Ms" << std::endl;

		pthread_mutex_init(&_dinosToBeUpdated_mutex, NULL);
		pthread_mutex_init(&_dinosUpdated_mutex, NULL);

		for (int i = 0; i < NUM_THREADS; ++i) {
			pthread_create(&dinoUpdateThreads[i], NULL, &DinoManager::DinoUpdateThread, this);
		}
	}
	else {
		DinoBuilder(amountOfDinos, 0);

		std::cout << "Time taken to initialise Dinos (non threaded): " << clock.getElapsedTime().asMilliseconds() << "Ms" << std::endl;
	}
}

void DinoManager::DinoBuilder(int amount, int iteration) {

	// This was used because rand was not thread safe - Thus numbers were the same
	std::uniform_int_distribution<int> screenXDistribution(0, SCREENX);
	std::uniform_int_distribution<int> screenYDistribution(0, SCREENY);
	std::uniform_int_distribution<int> dinoNameDistribution(0, 3);

	for (int i = 0; i < amount; ++i) {

		int dinoIndex = (iteration * amount) + i;

		std::mt19937 generator(dinoIndex);

		int x = screenXDistribution(generator);
		int y = screenYDistribution(generator);
		int dinoNameIndex = dinoNameDistribution(generator);

		Dino* dino = new Dino(x, y, _holder[_dinoNames[dinoNameIndex]]);

		if (IS_THREADED) {
			pthread_mutex_lock(&_buf_mutex);
			{
				_dinos.push_back(dino);
			}
			pthread_mutex_unlock(&_buf_mutex);
		}
		else {
			_dinos.push_back(dino);
		}
	}
}

void DinoManager::Update(float dTime) {
	sf::Clock clock;

	if (IS_THREADED) {
		if (THREADING_UPDATE_METHOD == 1) {
			_currentDTime = dTime;
			int groupSize = _dinos.size() / NUM_THREADS;

			std::vector<std::vector<Dino*>> groups;

			for (size_t i = 0; i < _dinos.size(); i += groupSize) {
				auto last = std::min(_dinos.size(), i + groupSize);
				groups.emplace_back(_dinos.begin() + i, _dinos.begin() + last);
			}

			pthread_mutex_lock(&_dinosToBeUpdated_mutex);
			{
				_groups = groups;
			}
			pthread_mutex_unlock(&_dinosToBeUpdated_mutex);

			while (_groupsFinished.size() != groups.size()) {
				auto test = "";
			}

			pthread_mutex_lock(&_dinosToBeUpdated_mutex);
			{
				_groups.clear();
			}
			pthread_mutex_unlock(&_dinosToBeUpdated_mutex);
			pthread_mutex_lock(&_dinosUpdated_mutex);
			{
				_groupsFinished.clear();
			}
			pthread_mutex_unlock(&_dinosUpdated_mutex);
		}

		if (THREADING_UPDATE_METHOD == 2) {
			_currentDTime = dTime;
			int size = _dinos.size();
			pthread_mutex_lock(&_dinosToBeUpdated_mutex);
			{
				_dinosToBeUpdated = std::vector<Dino*>(_dinos);
			}
			pthread_mutex_unlock(&_dinosToBeUpdated_mutex);

			//Wait for it to be empty
			while (_dinosFinishedUpdating.size() != size) {
				auto test = "";
			}
			pthread_mutex_lock(&_dinosToBeUpdated_mutex);
			{
				_dinosToBeUpdated.clear();
			}
			pthread_mutex_unlock(&_dinosToBeUpdated_mutex);
			pthread_mutex_lock(&_dinosUpdated_mutex);
			{
				_dinosFinishedUpdating.clear();
			}
			pthread_mutex_unlock(&_dinosUpdated_mutex);
		}
	}
	else {
		for (auto dino : _dinos) {
			dino->Update(dTime);
		}
	}
}

void DinoManager::UpdateThreadedPool() {
	if (THREADING_UPDATE_METHOD == 1) {
		while (true) {
			bool found = false;
			std::vector<Dino*> dinos;
			pthread_mutex_lock(&_dinosToBeUpdated_mutex);
			{
				if (!_groups.empty()) {
					found = true;
					dinos = _groups.back();
					_groups.pop_back();
				}
				else {
					found = false;
				}
			}
			pthread_mutex_unlock(&_dinosToBeUpdated_mutex);
			if (found) {
				for (auto dino : dinos) {
					dino->Update(_currentDTime);
				}
				pthread_mutex_lock(&_dinosUpdated_mutex);
				{
					_groupsFinished.push_back(dinos);
				}
				pthread_mutex_unlock(&_dinosUpdated_mutex);
			}
		}
	}

	if (THREADING_UPDATE_METHOD == 2) {
		while (true) {
			Dino* dino = nullptr;
			pthread_mutex_lock(&_dinosToBeUpdated_mutex);
			{
				if (!_dinosToBeUpdated.empty()) {
					dino = _dinosToBeUpdated.back();
					_dinosToBeUpdated.pop_back();
				}
			}
			pthread_mutex_unlock(&_dinosToBeUpdated_mutex);
			if (dino != nullptr) {

				dino->Update(_currentDTime);

				pthread_mutex_lock(&_dinosUpdated_mutex);
				{
					_dinosFinishedUpdating.push_back(dino);
				}
				pthread_mutex_unlock(&_dinosUpdated_mutex);
			}
		}
	}
}

void DinoManager::Draw(float deltaTime, sf::RenderWindow& window) {
	for (int i = 0; i < _dinos.size(); ++i) {
		window.draw(*_dinos[i]);
	}
}
