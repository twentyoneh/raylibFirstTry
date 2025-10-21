#pragma once
#include "ICommand.h"
#include <memory>
#include "commands/MoveCommands.h"
#include "commands/ShotCommands.h"
#include "raylib.h"

class InputHandler 
{
	public:
		InputHandler() = default;

		std::unique_ptr<ICommand> handleInput();
	
	private:
};