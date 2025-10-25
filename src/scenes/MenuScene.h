#pragma once
#include <vector>
#include "TypedScene.h"
#include "MenuSceneContext.h"
#include "../config/Button.h"
#include "../config/MenuInput.h"
#include "PlayScene.h"

class MenuScene : public TypedScene<MenuScheneContext> {
public:
	using TypedScene::TypedScene;

	void onEnterT(MenuScheneContext& ctx) override;
	void onExitT(MenuScheneContext& ctx) override {};
	void handleInputT(MenuScheneContext& ctx) override;
	void drawT(MenuScheneContext& ctx) const override;
	Transition updateT(MenuScheneContext& ctx, float dt) override;

private:
	std::vector<Button> buttons_;
	int focused_ = 0;
	bool wantStart_ = false;
	bool wantOptions_ = false;
	bool wantExit_ = false;
};