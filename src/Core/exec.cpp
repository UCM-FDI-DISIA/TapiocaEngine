#define EXEC

#ifdef EXEC
#include "Core.h"

int main(int argc, char** argv) {
    Tapioca::Game* game = new Tapioca::Game();

	Tapioca::Scene* sc = new Tapioca::Scene();

	Tapioca::GameObject* obj = new Tapioca::GameObject(sc);

	Tapioca::Component* comp = new Tapioca::Component(obj, "aux");

	game->run();

	// Al borrar el juego, se borran todas sus escenas
	// Al borrar una escena, se borran todos sus objetos
	// Al borrar un objeto, se borran todos sus componentes
	delete game;
}
#endif