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

	_grid->Update(dTime);
	sf::Clock clock;

	_dinoManager->Update(dTime);

	std::cout << clock.restart().asMicroseconds() << std::endl;

}

void GameManager::Draw(float dTime, sf::RenderWindow& window) {
	_grid->Draw(dTime, window);
	_dinoManager->Draw(dTime, window);
}
