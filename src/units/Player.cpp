#include "Player.h"
#include <raymath.h>


Player::Player(const std::string& name, float x, float y)
	: GameActor(x, y), name(name), health(100), speed(10)
{
	
}

Player::~Player()
{

}

void Player::AMove() { x -= speed; }

void Player::SMove() { y += speed; }

void Player::DMove() { x += speed; }

void Player::WMove() { y -= speed; }

void Player::Shot()
{
	std::cout << name << " fired a shot!" << std::endl;
	Vector2 mousePos = GetMousePosition();
	Vector2 dir = Vector2Normalize(Vector2Subtract(mousePos, getPosition()));
	float rotation = atan2f(dir.y, dir.x) * (180.0f / PI);
}

void Player::Update(float deltaTime)
{

}

void Player::Draw()
{
	DrawRectangle(x - 20, y - 20, 40, 40, BLUE);
}

