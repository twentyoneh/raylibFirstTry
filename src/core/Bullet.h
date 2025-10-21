#pragma once
#include "GameActor.h"
#include <cmath>
#include "raylib.h"

class Bullet : public GameActor
{
public:
	Bullet(int x, int y, float rotation, float speed = 100.0f);
	~Bullet();

	void Update(float deltaTime) override;

	void Draw() override;

private:
	float rotation_;
	float speed_;
	bool isActive_ = true;
};
