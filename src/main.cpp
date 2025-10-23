
#include "raylib.h"
#include <iostream>
#include "scenes/MenuScene.h"
#include "scenes/SceneContext.h"
#include "scenes/SceneStack.h"
#include "scenes/MenuSceneContext.h"

int main(void)
{
	InitWindow(1280, 720, "Scenes + Menu");
	SetTargetFPS(60);

	MenuScheneContext ctx;
	ctx.screenW = GetScreenWidth();
	ctx.screenH = GetScreenHeight();
	

	SceneStack stack(ctx);
	stack.push(std::make_unique<MenuScene>(ctx));

	while (!WindowShouldClose()) {
		float dt = GetFrameTime();

		stack.handleInput();
		stack.update(dt);

		if (stack.wantsExit()) break;

		BeginDrawing();
		stack.draw();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}