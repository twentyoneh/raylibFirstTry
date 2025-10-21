#include "Command.h"

class InputHandler {
public:
	Command* handleInput();

private:
	Command* buttonW_;
	Command* buttonA_;
	Command* buttonS_;
	Command* buttonD_;
};