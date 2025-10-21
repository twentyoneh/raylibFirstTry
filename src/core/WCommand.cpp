#include "Command.h"
#include <stdexcept>

class WCommand : public Command
{
public:
	virtual void execute(GameActor& actor);

};

void WCommand::execute(GameActor& actor) {
	actor.WMove();
}