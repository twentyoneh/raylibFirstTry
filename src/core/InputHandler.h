#pragma once
#include "ICommand.h"

class InputHandler 
{
	public:
		ICommand* handleInput();
	
	private:
		ICommand* buttonW_;
		ICommand* buttonA_;
		ICommand* buttonS_;
		ICommand* buttonD_;
};