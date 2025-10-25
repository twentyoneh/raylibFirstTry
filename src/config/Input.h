#pragma once
#include "Action.h"

class Input
{
public:
	virtual void poll() = 0;
	virtual bool pressed(Action a) const = 0;
	virtual bool down(Action a) const = 0;

protected:
	bool pUp = false, pDown = false, pLeft = false, pRight = false;
	bool dUp = false, dDown = false, dLeft = false, dRight = false;

};