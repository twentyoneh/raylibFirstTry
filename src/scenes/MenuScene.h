#pragma once
#include <vector>
#include "Scene.h"
#include "SceneContext.h"
#include "../config/Button.h"
#include "../config/MenuInput.h"

class MenuScene : public Scene {
public:
	void onEnter(SceneContext& ctx) override;
	void handleInput(SceneContext& ctx) override;
	Transition update() override;
	void draw(SceneContext& ctx) const override;

private:
	std::vector<Button> buttons_;
	int focused_ = 0;
	bool wantStart_ = false;
	bool wantOptions_ = false;
	bool wantExit_ = false;
};