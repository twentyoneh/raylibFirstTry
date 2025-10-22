#include "MenuScene.h"
#include <raylib.h>
// #include "PlayScene.hpp" // когда появится

void MenuScene::onEnter(SceneContext& ctx) {
	float w = ctx.ui.btnW, h = ctx.ui.btnH, s = ctx.ui.spacing;
	float x = (ctx.screenW - w) / 2.f;
	float y = (ctx.screenH - (3 * h + 2 * s)) / 2.f;
	buttons_.clear();
	buttons_.emplace_back("PLAY", Rectangle{ x,y,w,h });
	buttons_.emplace_back("OPTIONS", Rectangle{ x,y + h + s,w,h });
	buttons_.emplace_back("EXIT", Rectangle{ x,y + 2 * (h + s),w,h });
}

void MenuScene::handleInput(SceneContext& ctx) {
	ctx.input.poll();
	if (ctx.input.pressed(Action::Down)) focused_ = (focused_ + 1) % buttons_.size();
	if (ctx.input.pressed(Action::Up))   focused_ = static_cast<int>((focused_ - 1 + buttons_.size()) % buttons_.size());

	if (ctx.input.pressed(Action::Confirm)) {
		if (focused_ == 0) wantStart_ = true;
		else if (focused_ == 1) wantOptions_ = true;
		else if (focused_ == 2) wantExit_ = true;
	}
	if (ctx.input.pressed(Action::Back)) wantExit_ = true;
}

Transition MenuScene::update(SceneContext&, float) {
	// Клики мышью по кнопкам
	if (buttons_[0].draw(UiMenuTheme{}, false)) wantStart_ = true;     // рисовать в draw() правильнее; здесь лень копировать тему
	if (buttons_[1].draw(UiMenuTheme{}, false)) wantOptions_ = true;
	if (buttons_[2].draw(UiMenuTheme{}, false)) wantExit_ = true;

	if (wantStart_) {
		// return Transition::Swap([]{ return std::make_unique<PlayScene>(); });
		return Transition::None(); // пока PlayScene нет
	}
	if (wantOptions_) {
		// return Transition::Push([]{ return std::make_unique<OptionsScene>(); });
		wantOptions_ = false; // заглушка
	}
	if (wantExit_) {
		CloseWindow();
	}
	return Transition::None();
}

void MenuScene::draw(SceneContext& ctx) const {
	DrawText("TOP-DOWN SHOOTER", 40, 40, ctx.ui.titleSize, ctx.ui.title);
	for (int i = 0; i < (int)buttons_.size(); ++i) {
		const_cast<Button&>(buttons_[i]).draw(ctx.ui, i == focused_);
	}
}