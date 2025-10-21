
#include "raylib.h"
#include <iostream>
#include "core/Player.h"
#include "core/InputHandler.h"


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
		float deltaTime = GetFrameTime();
		// Обработка ввода
		std::unique_ptr<ICommand> command = inputHandler.handleInput();
		if(command) command->execute(player);

		player.Update(deltaTime);

		BeginDrawing();

		ClearBackground(RAYWHITE);
		player.Draw();

		EndDrawing();
	}
	CloseWindow();

	return 0;
}