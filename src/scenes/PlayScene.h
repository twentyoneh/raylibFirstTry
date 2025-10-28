#pragma once
#include "Scene.h"
#include "TypedScene.h"
#include "PlaySceneContext.h"
#include "../units/Player.h"
#include "../units/Bullet.h"

class PlayScene : public TypedScene<PlaySceneContext>
{
public:
	PlayScene(PlaySceneContext& ctx) : TypedScene<PlaySceneContext>(ctx), context(ctx) {}

	void onEnterT(PlaySceneContext& ctx) override;
	void onExitT(PlaySceneContext& ctx) override;
	void handleInputT(PlaySceneContext& ctx) override;
	void drawT(PlaySceneContext& ctx) const override;
	Transition updateT(PlaySceneContext& ctx, float dt) override;

private:
	PlaySceneContext& context;
	Player player_{ "Hero", 200.f, 200.f };
	Camera2D cam_{};
	bool wantExit_{ false };

	std::vector<Bullet> bullets_;

	void spawnBullet_(Vector2 origin, Vector2 dir, ProjectileKind kind);
	void updateBullets_(float dt);
	void drawBullets_() const;
};
