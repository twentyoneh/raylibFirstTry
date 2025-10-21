#pragma once
#include <stdexcept>

class GameActor
{
	public:
		GameActor(float x = 0.0f, float y = 0.0f);
		virtual ~GameActor();
	
		//TODO: ���������� ����� ��� ��� �� �������� � �������� ��� �������... 
		virtual void AMove();
		virtual void SMove();
		virtual void DMove();
		virtual void WMove();

		virtual void Update(float deltaTime) {};
		virtual void Draw() {};

		int getX() const { return x; };
		int getY() const { return y; };
		void setPosition(float newX, float newY) { x = newX; y = newY; };

	protected:
		int x;
		int y;
};