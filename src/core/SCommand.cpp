#include "ICommand.h"

class SCommand : public ICommand
{
public:
	void execute(GameActor& actor) override;

};

void SCommand::execute(GameActor& actor) {
	actor.SMove();
}