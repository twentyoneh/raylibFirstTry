#include "Player.h"


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

void Player::Update()
{

}

