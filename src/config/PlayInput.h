#pragma once
#include "raylib.h"
#include "Input.h"
#include "Action.h"

class PlayInput : public Input
{
public:
	void poll() override;
	bool pressed(Action a) const override;
	bool down(Action a) const override;

private:
	bool pFire = false, pRun = false, pPause = false;
	bool dFire = false, dRun = false, dPause = false;
};
