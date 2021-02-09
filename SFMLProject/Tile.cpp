#include "Tile.h"
#include <iostream>
#include "GameManager.h"

Tile::Tile(int x, int y, sf::Color colour) : _x(x), _y(y), _colour(colour) {
	_rect.setSize(sf::Vector2f(GRID_WIDTH, GRID_WIDTH));
	_rect.setPosition(sf::Vector2f(_x, _y));
	_rect.setFillColor(_colour);

	_xMax = x + GRID_WIDTH;
	_yMax = y + GRID_WIDTH;
}

void Tile::Update(float dTime) {


	//if (_colour == sf::Color::Black) {
	//	auto dinos = GameManager::GetInstance()->GetDinoManager()->GetDinos();

	//	for (int i = 0; i < dinos.size(); ++i) {
	//		sf::IntRect rect = sf::IntRect(_x, _y, GRID_WIDTH, GRID_WIDTH);
	//		sf::IntRect dinorect = sf::IntRect(dinos[i]->position.x, dinos[i]->position.y, DINOSIZEX, DINOSIZEY);
	//		if (rect.intersects(dinorect)) {
	//			// COllision with Dino
	//		}
	//	}
	//}
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_rect);
}