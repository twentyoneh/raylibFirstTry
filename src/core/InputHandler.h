#pragma once
#include "ICommand.h"
#include <memory>
#include "commands/MoveCommands.h"
#include "raylib.h"

class InputHandler 
{
	public:
		InputHandler() = default;

		std::unique_ptr<ICommand> handleInput();

		/*void setButtonW(ICommand* command) { buttonW_ = command; }
		void setButtonA(ICommand* command) { buttonA_ = command; }
		void setButtonS(ICommand* command) { buttonS_ = command; }
		void setButtonD(ICommand* command) { buttonD_ = command; }*/
	
	private:
		/*ICommand* buttonW_;
		ICommand* buttonA_;
		ICommand* buttonS_;
		ICommand* buttonD_;*/
};