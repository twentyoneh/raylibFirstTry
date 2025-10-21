#include "Command.h"
#include <stdexcept>

class SCommand : public Command
{
public:
	virtual void execute(GameActor& actor);

};

void SCommand::execute(GameActor& actor) {
	actor.SMove();
}