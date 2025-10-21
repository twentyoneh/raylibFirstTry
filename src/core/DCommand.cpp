#include "Command.h"
#include <stdexcept>

class DCommand : public Command
{
public:
	virtual void execute(GameActor& actor);

};

void DCommand::execute(GameActor& actor) {
	actor.DMove();
}