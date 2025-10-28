#include "PlayScene.h"

// то что должно прогружатьс€ в начале сцены игры
void PlayScene::onEnterT(PlaySceneContext& ctx)
{
	cam_.target = player_.getPosition();
	cam_.offset = { ctx.screenW * 0.5f, ctx.screenH * 0.5f };
	cam_.rotation = 0.0f;
	cam_.zoom = 3.0f;
}

void PlayScene::onExitT(PlaySceneContext& ctx)
{
	


}

void PlayScene::handleInputT(PlaySceneContext& ctx)
{
	ctx.input.poll();
	if (ctx.input.down(Action::Down)) {
		player_.SMove();
	}
	if (ctx.input.down(Action::Up)) {
		player_.WMove();
	}
	if (ctx.input.down(Action::Left)) {
		player_.AMove();
	}
	if (ctx.input.down(Action::Right)) {
		player_.DMove();
	}
	if (ctx.input.down(Action::Fire)) {
		Vector2 p = player_.getPosition();
		Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), cam_);
		spawnBullet_(p, Vector2Subtract(mouseWorld, p), ProjectileKind::BasicBullet);
	}
	if (ctx.input.pressed(Action::Run)) {

	}
	if (ctx.input.pressed(Action::Pause)) {
		wantExit_ = true;
	}
}

Transition PlayScene::updateT(PlaySceneContext& ctx, float dt)
{
	updateBullets_(dt);

	Vector2 target = player_.getPosition();
	cam_.target.x += (target.x - cam_.target.x) * (1.0f - powf(0.001f, dt)); // сглаживание
	cam_.target.y += (target.y - cam_.target.y) * (1.0f - powf(0.001f, dt));

	if (wantExit_) {
		return Transition::Exit();
	}
	return Transition::None();
}

void PlayScene::drawT(PlaySceneContext& ctx) const
{
	ClearBackground(Color{ 18,18,22,255 });

	BeginMode2D(cam_);
	for (int x = -2000; x <= 2000; x += 64) DrawLine(x, -2000, x, 2000, Color{ 40,40,48,255 });
	for (int y = -2000; y <= 2000; y += 64) DrawLine(-2000, y, 2000, y, Color{ 40,40,48,255 });

	player_.Draw();
	drawBullets_();

	EndMode2D();

	//DrawText("WASD Ч ходьба, Shift Ч бег, Ћ ћ Ч выстрел, Esc Ч назад",
		//20, ctx.screenH - 36, 18, RAYWHITE);
}


void PlayScene::spawnBullet_(Vector2 origin, Vector2 dir, ProjectileKind kind) {
	bullets_.push_back(context.bullets.make(origin, dir, kind));
}

void PlayScene::updateBullets_(float dt) {
	for (auto& b : bullets_) if (b.active) {
		b.pos.x += b.vel.x * dt;
		b.pos.y += b.vel.y * dt;
		b.ttl -= dt;
		if (b.ttl <= 0.f) b.active = false;
		// TODO: столкновени€/границы мира/ущерб
	}
	bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(),
		[](const Bullet& x) { return !x.active; }), bullets_.end());
}

void PlayScene::drawBullets_() const {
	for (auto& b : bullets_) if (b.active) {
		DrawCircleV(b.pos, b.radius, YELLOW);
	}
}
