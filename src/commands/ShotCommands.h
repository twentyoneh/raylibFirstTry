#include "../core/ICommand.h"
#include "../core/Player.h"

class ShootCommand : public ICommand {
public:
	void execute(Player& player) override { player.Shot(); }
};