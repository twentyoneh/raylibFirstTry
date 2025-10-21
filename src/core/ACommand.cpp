#include "Command.h"
#include <stdexcept>

class ACommand : public Command
{
public:
	virtual void execute(GameActor& actor);

};

void ACommand::execute(GameActor& actor) {
	actor.AMove();
}