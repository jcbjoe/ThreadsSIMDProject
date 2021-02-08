#include "GameManager.h"
#include <iostream>

GameManager::GameManager() {
	_grid = new Grid();
	_dinoManager = new DinoManager();

	_dinoManager->InitialiseDinos(AMOUNTOFDINOS);
}


GameManager* GameManager::GetInstance()
{
	static GameManager instance;

	return &instance;
}

void GameManager::Update(float dTime) {
	sf::Clock clock;
	_grid->Update(dTime);
	std::cout << "GridUpdate: " << clock.restart().asSeconds() << std::endl;

	_dinoManager->Update(dTime);
	std::cout << "DinoManagerUpdate: " << clock.restart().asSeconds() << std::endl;
}

void GameManager::Draw(float dTime, sf::RenderWindow& window) {
	sf::Clock clock;
	_grid->Draw(dTime, window);
	std::cout << "DrawGrid: " << clock.restart().asSeconds() << std::endl;
	_dinoManager->Draw(dTime, window);
	std::cout << "DrawDinoManager: " << clock.restart().asSeconds() << std::endl;
}
