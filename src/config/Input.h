#pragma once
#include "Action.h"

// Интерфейс для обработки ввода
// Реализуется для конкретных сцен
class Input
{
public:
	// Об
	virtual void poll() = 0;
	virtual bool pressed(Action a) const = 0;
	virtual bool down(Action a) const = 0;

protected:
	bool pUp = false, pDown = false, pLeft = false, pRight = false;
	bool dUp = false, dDown = false, dLeft = false, dRight = false;

};