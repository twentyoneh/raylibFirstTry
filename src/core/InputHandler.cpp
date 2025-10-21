#include "InputHandler.h"
#include "raylib.h"


ICommand* InputHandler::handleInput() {
	if (IsKeyPressed(KEY_W)) return buttonW_;
	if (IsKeyPressed(KEY_A)) return buttonA_;
	if (IsKeyPressed(KEY_S)) return buttonS_;
	if (IsKeyPressed(KEY_D)) return buttonD_;

	return NULL;
}