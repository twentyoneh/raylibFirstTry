#include "BulletManager.h"

void BulletManager::addBullet(const Vector2 position, float rotation, float speed)
{
	bullets.emplace_back(position.x, position.y, rotation, speed);
}

void BulletManager::Update(float deltaTime)
{
	for (auto& b : bullets)
		b.Update(deltaTime);

	bullets.erase(
		std::remove_if(bullets.begin(), bullets.end(),
			[](const Bullet& b) {
				Vector2 pos = b.getPosition();
				return (pos.x < 0 || pos.x > GetScreenWidth() ||
					pos.y < 0 || pos.y > GetScreenHeight());
			}),
		bullets.end());
}

void BulletManager::Draw()
{
	for (auto& b : bullets)
		b.Draw();
}

