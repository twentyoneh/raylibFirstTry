#pragma once
#include "raylib.h"
#include "Input.h"
#include "Action.h"

class MenuInput : public Input 
{
public:
	void poll() override;
	bool pressed(Action a) const override;
	bool down(Action a) const override;

private:
	bool pConfirm = false, pBack = false;
	bool dConfirm = false, dBack = false;
};