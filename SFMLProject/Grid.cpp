#include "Grid.h"

Grid::Grid() {

	gridLayout = std::vector<std::vector<Tile*>>(GRID_SQUARES_X, std::vector<Tile*>(GRID_SQUARES_Y, nullptr));

	for (int y = 0; y < GRID_SQUARES_Y; ++y) {

		const int yCoord = y * GRID_WIDTH;

		for (int x = 0; x < GRID_SQUARES_X; ++x) {

			const int xCoord = x * GRID_WIDTH;

			int random = rand() % 10;

			sf::Color col = sf::Color::White;

			if (random == 1)col = sf::Color::Black;

			gridLayout[x][y] = new Tile(xCoord, yCoord, col);
		}
	}

}

void Grid::Update(float dTime) {
	for (int i = 0; i < gridLayout.size(); ++i) {
		for (int j = 0; j < gridLayout[i].size(); ++j) {
			gridLayout[i][j]->Update(dTime);
		}
	}
}

void Grid::Draw(float deltaTime, sf::RenderWindow& window) {

	for (int i = 0; i < gridLayout.size(); ++i) {
		for (int j = 0; j < gridLayout[i].size(); ++j) {
			auto tile = gridLayout[i][j];
			if (tile->GetColour() == sf::Color::Black || tile->GetColour() == sf::Color::Red) {
				window.draw(*tile);
			}
		}
	}
}

Tile* Grid::GetTileAtPos(int x, int y) {
	if (x < 0 || x >= SCREENX || y < 0 || y >= SCREENY) return nullptr;

	const int xGrid = ceil(x / GRID_WIDTH);
	const int yGrid = ceil(y / GRID_WIDTH);

	return  gridLayout[xGrid][yGrid];
}