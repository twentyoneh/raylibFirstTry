#include "MenuScene.h"
#include <raylib.h>

void MenuScene::onEnterT(MenuScheneContext& ctx) {
	float w = ctx.ui.btnW, h = ctx.ui.btnH, s = ctx.ui.spacing;
	float x = (ctx.screenW - w) / 2.f;
	float y = (ctx.screenH - (3 * h + 2 * s)) / 2.f;
	buttons_.clear();
	buttons_.emplace_back("PLAY", Rectangle{ x,y,w,h });
	buttons_.emplace_back("OPTIONS", Rectangle{ x,y + h + s,w,h });
	buttons_.emplace_back("EXIT", Rectangle{ x,y + 2 * (h + s),w,h });
}

void MenuScene::handleInputT(MenuScheneContext& ctx) {
	ctx.input.poll();
	if (ctx.input.pressed(Action::Down)) 
		focused_ = (focused_ + 1) % buttons_.size();
	if (ctx.input.pressed(Action::Up))   
		focused_ = static_cast<int>((focused_ - 1 + buttons_.size()) % buttons_.size());

	if (ctx.input.pressed(Action::Confirm)) {
		if (focused_ == 0) wantStart_ = true;
		else if (focused_ == 1) wantOptions_ = true;
		else if (focused_ == 2) wantExit_ = true;
	}
	if (ctx.input.pressed(Action::Back)) wantExit_ = true;
}

Transition MenuScene::updateT(MenuScheneContext& ctx, float dt) {
	if (wantStart_) {
		return Transition::Swap([&ctx]{
			PlaySceneContext* playCtx = new PlaySceneContext{};
			return std::unique_ptr<Scene>(new PlayScene(*playCtx));
			});
	}
	if (wantOptions_) {
		// return Transition::Push([]{ return std::make_unique<OptionsScene>(); });
		wantOptions_ = false;
	}
	if (wantExit_) {
		return Transition::Exit();
	}
	return Transition::None();
}

void MenuScene::drawT(MenuScheneContext& ctx) const {
	if (wantExit_) return;
	DrawText("TOP-DOWN SHOOTER", 40, 40, ctx.ui.titleSize, ctx.ui.title);
	ClearBackground(ctx.ui.bg);
	for (int i = 0; i < (int)buttons_.size(); ++i) {
		buttons_[i].draw(ctx.ui, i == focused_);
	}
}