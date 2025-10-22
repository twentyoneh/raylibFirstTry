#include "Button.h"

bool Button::draw(const UiMenuTheme& theme, bool focused) {
	Vector2 m = GetMousePosition();
	bool hover = CheckCollisionPointRec(m, rect_) || focused;

	DrawRectangleRec(rect_, hover ? theme.btnHover : theme.btn);
	DrawRectangleLinesEx(rect_, 2.f, theme.btnBorder);

	int fs = theme.btnSize;
	int tw = MeasureText(text_.c_str(), fs);
	DrawText(text_.c_str(),
		int(rect_.x + (rect_.width - tw) / 2),
		int(rect_.y + (rect_.height - fs) / 2), fs, RAYWHITE);

	return hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}