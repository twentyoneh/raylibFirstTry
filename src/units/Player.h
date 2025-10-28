#pragma once
#include "GameActor.h"
#include "raylib.h"
#include <string>
#include <iostream>
#include <vector>
#include <raymath.h>

class Player : public GameActor
{
	public:
		Player(const std::string& name, float x = 0.0f, float y = 0.0f);
		~Player() override;

		void AMove() override;
		void SMove() override;
		void DMove() override;
		void WMove() override;

		void Shot() override;

		void Update(float deltaTime) override;
		void Draw() const override;

		std::string getName() const { return name; };
		void setSpeed(int speed);
	private:
		std::string name;
		float health;
		int speed;
		Texture2D texture;
};