#pragma once

constexpr int SCREENX = 1200;
constexpr int SCREENY = 1080;

constexpr int GRID_SQUARES_X = 25;
constexpr int GRID_WIDTH = (SCREENX / GRID_SQUARES_X);
const int GRID_SQUARES_Y = ceil((float)SCREENY / (float)GRID_WIDTH); // Edgecase when Y is a decimal - In this case 22.5 causing issues with the grid

constexpr int DINOTEXSIZEX = 24;
constexpr int DINOTEXSIZEY = 24;

constexpr int DINOSCALE = 2;

constexpr int DINOSIZEX = DINOTEXSIZEX * DINOSCALE;
constexpr int DINOSIZEY = DINOTEXSIZEY * DINOSCALE;

constexpr unsigned int AMOUNTOFDINOS = 10000;
constexpr unsigned int NUM_THREADS = 4;

constexpr unsigned int GROUP_SIZE = AMOUNTOFDINOS / NUM_THREADS;

constexpr bool IS_THREADED = true;

constexpr int THREADING_UPDATE_METHOD = 1;