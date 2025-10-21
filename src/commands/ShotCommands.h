#include "../core/ICommand.h"
#include "../core/Player.h"

class ShootCommand : public ICommand {
public:
	void execute(GameActor& player, BulletManager& bulletManager) override { player.Shot(bulletManager); }
};