#pragma once
#include <stdexcept>
#include "raylib.h"

class GameActor
{
	public:
		GameActor(float x = 0.0f, float y = 0.0f);
		virtual ~GameActor();
	 
		virtual void AMove();
		virtual void SMove();
		virtual void DMove();
		virtual void WMove();

		virtual void Shot() { throw std::runtime_error("Shot method not implemented for this actor."); };

		virtual void Update(float deltaTime) {};
		virtual void Draw() const {};

		int getX() const { return x; };
		int getY() const { return y; };
		void setPosition(int newX, int newY) { x = newX; y = newY; };
		Vector2 getPosition() const { return Vector2{ static_cast<float>(x), static_cast<float>(y) }; };

	protected:
		int x;
		int y;
};