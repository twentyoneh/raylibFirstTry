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

		virtual void Update() {};

		float getX() const { return x; };
		float getY() const { return y; };
		float setPosition(float newX, float newY) { x = newX; y = newY; };

	protected:
		float x;
		float y;
};