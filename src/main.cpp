
#include "raylib.h"


const int screenWidth = 800;
const int screenHeight = 450;

int main(void)
{
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - input mouse");

	SetTargetFPS(60);

	// Main game loop
	while (!WindowShouldClose()) 
	{
		
	}
	CloseWindow();

	return 0;
}