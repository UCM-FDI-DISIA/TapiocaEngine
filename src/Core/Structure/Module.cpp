#include "Module.h"
#include "Game.h"

namespace Tapioca {
Module::Module() {
	Game::get()->addModule(this);
}
}