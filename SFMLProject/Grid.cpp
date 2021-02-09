#include "Grid.h"
#include <nmmintrin.h> // SSE4.2 intrinsics and below
#include <immintrin.h>

Grid::Grid() {

	gridLayout = std::vector<std::vector<Tile*>>(GRID_SQUARES_X, std::vector<Tile*>(GRID_SQUARES_Y, nullptr));

	if (USING_SIMD) {

		int y4Th = GRID_SQUARES_Y / 4;
		int x4Th = GRID_SQUARES_X / 4;
		int yCount = 0;

		__m128i x4thSim = _mm_set1_epi32(x4Th);
		__m128i xValues = _mm_setr_epi32(0, 1, 2, 3);

		xValues = _mm_mullo_epi32(xValues, x4thSim);

		__m128i gridWidthSim = _mm_set1_epi32(GRID_WIDTH);

		for (int y = 0; y < GRID_SQUARES_Y; ++y) {

			const int yCoord = y * GRID_WIDTH;
			int xCount = 0;
			for (int x = 0; x <= x4Th; ++x) {

				__m128i xCountSim = _mm_set1_epi32(xCount);
				__m128i xData = _mm_setr_epi32(xValues.m128i_u32[0], xValues.m128i_u32[1], xValues.m128i_u32[2], xValues.m128i_u32[3]);
				xData = _mm_add_epi32(xData, xCountSim);

				xCount++;

				__m128i xCoords = _mm_setr_epi32(xData.m128i_u32[0], xData.m128i_u32[1], xData.m128i_u32[2], xData.m128i_u32[3]);
				xCoords = _mm_mullo_epi32(xCoords, gridWidthSim);

				auto col = sf::Color::Red;

				gridLayout[xData.m128i_u32[0]][y] = new Tile(xCoords.m128i_u32[0], yCoord, col);
				gridLayout[xData.m128i_u32[1]][y] = new Tile(xCoords.m128i_u32[1], yCoord, col);
				gridLayout[xData.m128i_u32[2]][y] = new Tile(xCoords.m128i_u32[2], yCoord, col);
				gridLayout[xData.m128i_u32[3]][y] = new Tile(xCoords.m128i_u32[3], yCoord, col);
			}
		}
		auto done = "";
	}
	else {

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

	if (USING_SIMD) {
		__m128i gridWidth = _mm_set1_epi32(GRID_WIDTH);
		__m128i xyVal = _mm_setr_epi32(x, y, 1, 1);
		xyVal = _mm_div_epi32(xyVal, gridWidth);
		return  gridLayout[xyVal.m128i_u32[0]][xyVal.m128i_u32[1]];
	}
	else {
		const int xGrid = ceil(x / GRID_WIDTH);
		const int yGrid = ceil(y / GRID_WIDTH);
		return  gridLayout[xGrid][yGrid];
	}


}