
#include "raylib.h"
#include <iostream>
#include "core/Player.h"
#include "core/InputHandler.h"
#include "commands/MoveCommands.h"


const int screenWidth = 800;
const int screenHeight = 450;

int main(void)
{
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib example - input mouse");
	SetTargetFPS(60);

	Player player("player", screenWidth / 2.0f, screenHeight / 2.0f);
	InputHandler inputHandler = InputHandler();

	// Main game loop
	while (!WindowShouldClose()) 
	{
		auto command = inputHandler.handleInput();
		if(command) command->execute(player);

		BeginDrawing();

		ClearBackground(RAYWHITE);
		DrawCircle(player.getX(), player.getY(), 50, MAROON);

		EndDrawing();
	}
	CloseWindow();

	return 0;
}