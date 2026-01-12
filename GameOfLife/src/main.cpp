/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>
#include <time.h>

const int screenWidth = 800;
const int screenHeight = 800;

const int gridWidth = 80;
const int gridHeight = 80;

const float cellWidth = (float)screenWidth / (float)gridWidth;
const float cellHeight = (float)screenHeight / (float)gridHeight;

using Cells = std::vector<bool>;

bool ReadCell(int x, int y, const Cells& cells)
{
    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) return false;
    return cells[y * gridWidth + x];
}

void WriteCell(int x, int y, Cells& cells, bool value)
{
	if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) return;
	cells[y * gridWidth + x] = value;
}

void RandomizeCells(Cells& cells)
{
	for (int i = 0; i < cells.size(); i++)
	{
		cells[i] = (GetRandomValue(0, 1) == 1);
	}
}

int CountLiveNeighbors(int x, int y, const Cells& cells) {
	int count = 0;

	if (ReadCell(x - 1, y - 1, cells)) count++;
	if (ReadCell(x, y - 1, cells)) count++;
	if (ReadCell(x + 1, y - 1, cells)) count++;
	if (ReadCell(x - 1, y, cells)) count++;
	if (ReadCell(x + 1, y, cells)) count++;
	if (ReadCell(x - 1, y+1, cells)) count++;
	if (ReadCell(x, y+1, cells)) count++;
	if (ReadCell(x + 1, y+1, cells)) count++;

	return count;
}

int main ()
{
	// randomize seed value using the system clock
		SetRandomSeed((unsigned int)time(NULL));
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	// Create the window and OpenGL context
	InitWindow(screenWidth, screenHeight, "Game of Life");
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	// set the target fps, set to lower value to make generation slower (30, 20 or 10)
	SetTargetFPS(60);
	// initialize cells (grid width * grid height)
	Cells currentGeneration(gridWidth * gridHeight);
	Cells nextGeneration(gridWidth * gridHeight);
	// randomize current generation
	RandomizeCells(currentGeneration);
	// game loop
	while (!WindowShouldClose()) // run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// set all next generation cells to dead (false)
		std::fill(nextGeneration.begin(), nextGeneration.end(), false);
		// randomize cells if spacebar is pressed
		if (GetKeyPressed() == KEY_SPACE) {
		// randomize current generation
		RandomizeCells(currentGeneration);
		}
	// update next generation cells from current generation
	for (int y = 1; y < gridHeight - 1; y++) {
	for (int x = 1; x < gridWidth - 1; x++) {
	int count = CountLiveNeighbors(x, y, currentGeneration);
	bool isAlive = ReadCell(x, y, currentGeneration);
	// Conway's Game of Life rules:
	// 1. Live cell with 2-3 neighbors survives
	// 2. Dead cell with exactly 3 neighbors becomes alive
	// 3. All other cells die or stay dead
	if (isAlive) {
		// write true to next generation if survives
		if (count == 2 || count == 3) {
			WriteCell(x, y, nextGeneration, true);
		}
		else {
			// cell dies
			WriteCell(x, y, nextGeneration, false);
		}
		}
		else {
		// write true to next generation if becomes alive
		if (count == 3) WriteCell(x, y, nextGeneration, true);
		}
		}
		}
	// drawing
	BeginDrawing();
	// Setup the back buffer for drawing (clear color and depth buffers)
	ClearBackground(BLACK);
	// draw next generation cells
	// go through cells and if the cell is alive use draw rectangle
	// need to set x and y of rectangle using cell size (see below)
	// DRAW CELLS HERE
	for (int y = 0; y < gridHeight; y++) {
		for (int x = 0; x < gridWidth; x++) {
			if (ReadCell(x, y, nextGeneration)) {
				DrawRectangle((int)(x * cellWidth), (int)(y * cellHeight), (int)cellWidth + 1, (int)cellHeight + 1, WHITE);
			}
		}
	}

	DrawText("SPACE: Randomize", 10, 10, 20, GRAY);
	DrawFPS(10, 40);
	// end the frame and get ready for the next one (display frame, poll input, etc...)
	EndDrawing();
	// set the current generation to the next generation
	currentGeneration = nextGeneration;
	}
		// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;

}
