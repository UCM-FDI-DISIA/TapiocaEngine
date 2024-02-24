#include "Module.h"
#include "Game.h"

namespace Tapioca {
Module::Module() {
	Game* game = Game::get();
    game->addModule(this);
}
}