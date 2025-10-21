#include "ICommand.h"
#include"GameActor.h"


class DCommand : public ICommand
{
public:
	void execute(GameActor& actor) override;

};

void DCommand::execute(GameActor& actor) {
	actor.DMove();
}