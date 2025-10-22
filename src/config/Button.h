#pragma once
#include <string>
#include "raylib.h"
#include "UiMenuTheme.h"

class Button {
public:
	Button(std::string text, Rectangle rect) : text_(std::move(text)), rect_(rect) {}
	void setRect(Rectangle r) { rect_ = r; }
	bool draw(const UiMenuTheme& t, bool focused) const;

private:
	std::string text_;
	Rectangle rect_;
};