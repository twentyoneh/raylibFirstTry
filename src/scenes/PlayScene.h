#pragma once
#include "Scene.h"
#include "TypedScene.h"
#include "PlaySceneContext.h"
#include "../units/Player.h"

class PlayScene : public TypedScene<PlaySceneContext>
{
public:
	using TypedScene::TypedScene;

	void onEnterT(PlaySceneContext& ctx) override;
	void onExitT(PlaySceneContext& ctx) override;
	void handleInputT(PlaySceneContext& ctx) override;
	void drawT(PlaySceneContext& ctx) const override;
	Transition updateT(PlaySceneContext& ctx, float dt) override;

private:
	Player player_{ "Hero", 200.f, 200.f };
	//std::vector<Bullet> bullets_;
	Camera2D cam_{};

	// настроечки
	float moveSpeed_ = 220.f;
	float runMul_ = 1.5f;
	float bulletSpeed_ = 520.f;

	bool wantExit_ = false;
	//void shoot_(const PlaySceneContext& ctx);
	//void updateBullets_(float dt, const PlaySceneContext& ctx);
};
