#include "PlayInput.h"
#include "raylib.h"

void PlayInput::poll()
{
	dUp = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
	dDown = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
	dLeft = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
	dRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
	dFire = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	dRun = IsKeyDown(KEY_LEFT_SHIFT);
	dPause = IsKeyDown(KEY_ESCAPE);

	pUp = IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
	pDown = IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S);
	pLeft = IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A);
	pRight = IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D);
	pFire = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	pRun = IsKeyPressed(KEY_LEFT_SHIFT);
	pPause = IsKeyPressed(KEY_ESCAPE);
}

bool PlayInput::pressed(Action a) const
{
	switch (a) {
	case Action::Up: return pUp;
	case Action::Down: return pDown;
	case Action::Left: return pLeft;
	case Action::Right: return pRight;
	case Action::Fire: return pFire;
	case Action::Run: return pRun;
	case Action::Pause: return pPause;
	default: return false;
	}
}

bool PlayInput::down(Action a) const
{
	switch (a) {
	case Action::Up: return dUp;
	case Action::Down: return dDown;
	case Action::Left: return dLeft;
	case Action::Right: return dRight;
	case Action::Fire: return dFire;
	case Action::Run: return dRun;
	case Action::Pause: return dPause;
	default: return false;
	}
}
