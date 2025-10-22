#pragma once
#include "Scene.h"

class PlayScene : public Scene
{
public:
	void onEnter(SceneContext& ctx) override;
	void handleInput(SceneContext& ctx) override;
	Transition update() override;
	void draw(SceneContext& ctx) const override;

private:

};
