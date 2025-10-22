
#include "raylib.h"
#include <iostream>
#include "scenes/MenuScene.h"
#include "scenes/SceneContext.h"
#include "scenes/SceneStack.h"

int main(void)
{
	InitWindow(1280, 720, "Scenes + Menu");
	SetTargetFPS(60);

	SceneContext ctx;
	ctx.screenW = GetScreenWidth();
	ctx.screenH = GetScreenHeight();

	SceneStack stack(ctx);
	stack.push(std::make_unique<MenuScene>());

	while (!WindowShouldClose()) {
		float dt = GetFrameTime();

		stack.handleInput();
		stack.update(dt);

		BeginDrawing();
		ClearBackground(ctx.ui.bg);
		stack.draw();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}