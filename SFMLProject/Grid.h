#pragma once
#include <vector>
#include "Tile.h"
#include <SFML/Graphics.hpp>
#include "Config.h"

class Grid {

private:
	std::vector<std::vector<Tile*>> gridLayout;

public:

	Grid();

	void Update(float dTime);

	void Draw(float deltaTime, sf::RenderWindow& window);

	Tile* GetTileAtPos(int x, int y);

};