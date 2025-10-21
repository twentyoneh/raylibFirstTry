#pragma once
#include <vector>
#include "Bullet.h"

class BulletManager {
public:
	void addBullet(const Vector2 position, float rotation, float speed);
	void Update(float deltaTime);
	void Draw();

	const std::vector<Bullet>& getBullets() const { return bullets; }
private:
	std::vector<Bullet> bullets;
};