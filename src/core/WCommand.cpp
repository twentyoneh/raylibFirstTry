#include "ICommand.h"

class WCommand : public ICommand
{
public:
	void execute(GameActor& actor) override;

};

void WCommand::execute(GameActor& actor) {
	actor.WMove();
}