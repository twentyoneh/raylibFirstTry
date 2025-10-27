#include "PlayScene.h"

// то что должно прогружатьс€ в начале сцены игры
void PlayScene::onEnterT(PlaySceneContext& ctx)
{
	cam_.target = player_.getPosition();
	cam_.offset = { ctx.screenW * 0.5f, ctx.screenH * 0.5f };
	cam_.rotation = 0.0f;
	cam_.zoom = 1.0f;
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
	if (ctx.input.pressed(Action::Fire)) {

	}
	if (ctx.input.pressed(Action::Run)) {

	}
	if (ctx.input.pressed(Action::Pause)) {
		wantExit_ = true;
	}
}

Transition PlayScene::updateT(PlaySceneContext& ctx, float dt)
{
	//updateBullets_(dt, ctx);

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

	//const_cast<Player&>(player_).Draw(); // твой Player::Draw() без const
	player_.Draw();
	/*for (auto& b : bullets_) {
		DrawCircleV(b.pos, 4.f, YELLOW);
	}*/

	EndMode2D();

	//DrawText("WASD Ч ходьба, Shift Ч бег, Ћ ћ Ч выстрел, Esc Ч назад",
		//20, ctx.screenH - 36, 18, RAYWHITE);
}
