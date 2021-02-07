#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "DinoManager.h";
#include "Grid.h"

class GameManager {

private:
	Grid* _grid = nullptr;

	DinoManager* _dinoManager = nullptr;

	static GameManager* instance;

	GameManager();

	sf::Vector2f* _destinationClick = nullptr;

public:
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	static GameManager* GetInstance();

	void Update(float dTime);

	void Draw(float dTime, sf::RenderWindow& window);

	DinoManager* GetDinoManager() { return _dinoManager; };
	Grid* GetGrid() { return _grid; };

	void SetDestination(sf::Vector2f* vec) { _destinationClick = vec; };

	sf::Vector2f* GetDestination() { return _destinationClick; };

};