#include "ICommand.h"

class ACommand : public ICommand
{
public:
	void execute(GameActor& actor) override;

};

void ACommand::execute(GameActor& actor) {
	actor.AMove();
}