#pragma once
#include "raylib.h"

enum class Action { 
	Up, 
	Down, 
	Left, 
	Right, 
	Confirm, 
	Back, 
	None 
};

struct MenuInput {
	void poll();
	bool pressed(Action a) const;
	bool down(Action a) const;

private:
	bool pUp = false, pDown = false, pLeft = false, pRight = false, pConfirm = false, pBack = false;
	bool dUp = false, dDown = false, dLeft = false, dRight = false, dConfirm = false, dBack = false;
};