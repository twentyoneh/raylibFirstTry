#pragma once
#include "raylib.h"

struct UiMenuTheme {
	Color bg{ 10,10,14,255 };
	Color title{ 245,245,245,255 };
	Color btn{ 70,70,75,255 };
	Color btnHover{ 105,105,115,255 };
	Color btnBorder{ 220,220,220,255 };
	int   titleSize = 42;
	int   btnSize = 28;
	float btnW = 320.f;
	float btnH = 56.f;
	float spacing = 18.f;
};