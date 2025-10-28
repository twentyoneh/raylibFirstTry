#pragma once
#include "../core/ICommand.h"
#include "../core/GameActor.h"

class MoveUpCommand : public ICommand {
public:
	void execute(GameActor& actor) override { actor.WMove(); }
};

class MoveDownCommand : public ICommand {
public:
	void execute(GameActor& actor) override { actor.SMove(); }
};

class MoveLeftCommand : public ICommand {
public:
	void execute(GameActor& actor) override { actor.AMove(); }
};

class MoveRightCommand : public ICommand {
public:
	void execute(GameActor& actor) override { actor.DMove(); }
};
