#include "Bullet.h"

Bullet::Bullet(int x, int y, float rotation, float speed /*= 100.0f*/)
	: GameActor(x, y), rotation_(rotation), speed_(speed)
{
}

Bullet::~Bullet()
{

}

void Bullet::Update(float deltaTime)
{
	x += cosf(rotation_ * (PI / 180.0)) * speed_ * deltaTime;
	y += sinf(rotation_ * (PI / 180.0)) * speed_ * deltaTime;

	if (x < 0 || x > GetScreenWidth() || y < 0 || y > GetScreenHeight())
		isActive_= false;
}

void Bullet::Draw()
{
	DrawCircle(x, y, 5, RED);
}

