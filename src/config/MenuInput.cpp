#include "MenuInput.h"

// действия которые модно делать внутри меню
void MenuInput::poll() {
	dUp = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
	dDown = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
	dLeft = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
	dRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
	dConfirm = IsKeyDown(KEY_ENTER) || IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_LEFT_BUTTON);
	dBack = IsKeyDown(KEY_ESCAPE);

	pUp = IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
	pDown = IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S);
	pLeft = IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A);
	pRight = IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D);
	pConfirm = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	pBack = IsKeyPressed(KEY_ESCAPE);
}

bool MenuInput::pressed(Action a) const {
	switch (a) {
	case Action::Up: return pUp; 
	case Action::Down: return pDown;
	case Action::Left: return pLeft; 
	case Action::Right: return pRight;
	case Action::Confirm: return pConfirm; 
	case Action::Back: return pBack;
	default: return false;
	}
}
bool MenuInput::down(Action a) const {
	switch (a) {
	case Action::Up: return dUp; 
	case Action::Down: return dDown;
	case Action::Left: return dLeft; 
	case Action::Right: return dRight;
	case Action::Confirm: return dConfirm; 
	case Action::Back: return dBack;
	default: return false;
	}
}