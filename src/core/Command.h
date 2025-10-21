#include "GameActor.h"

class Command
{
public:
	virtual ~Command();
	virtual void execute(GameActor& actor) = 0;
};

Command::~Command() = default;

