#pragma once
#include "GameActor.h"

class ICommand
{
	public:
		virtual ~ICommand();
		virtual void execute(GameActor& actor) = 0;
};