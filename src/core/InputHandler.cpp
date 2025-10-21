#include "InputHandler.h"



std::unique_ptr<ICommand> InputHandler::handleInput() {
	if (IsKeyDown(KEY_W)) return std::make_unique<MoveUpCommand>();
	if (IsKeyDown(KEY_A)) return std::make_unique<MoveLeftCommand>();
	if (IsKeyDown(KEY_S)) return std::make_unique<MoveDownCommand>();
	if (IsKeyDown(KEY_D)) return std::make_unique<MoveRightCommand>();
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return std::make_unique<ShootCommand>();

	return nullptr;
}