#include "Player.h"
#include <raymath.h>
#include <filesystem>



Player::Player(const std::string& name, float x, float y)
	: GameActor(x, y), name(name), health(100), speed(10)
{
	std::cout << "fileDirectory: " << std::filesystem::current_path() << std::endl;
	texture = LoadTexture("resources/textures/units/player/Player0001.png");
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

void Player::Draw() const
{
	DrawTexture(texture, 32, 32, Color{ 18,18,22,255 });
	//DrawRectangleLines()
	//DrawRectangle(x - 20, y - 20, 40, 40, BLUE);
}

void Player::setSpeed(int speed)
{
	this->speed = speed;
}

