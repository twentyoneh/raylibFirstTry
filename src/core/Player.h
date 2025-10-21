#pragma once
#include "GameActor.h"
#include "raylib.h"
#include "Bullet.h"
#include <string>

class Player : public GameActor
{
	public:
		Player(const std::string& name, float x = 0.0f, float y = 0.0f);
		~Player() override;

		void AMove() override;
		void SMove() override;
		void DMove() override;
		void WMove() override;

		void Shot();

		void Update(float deltaTime) override;
		void Draw() override;

		std::string getName() const { return name; };
	private:
		std::string name;
		float health;
		float speed;
};