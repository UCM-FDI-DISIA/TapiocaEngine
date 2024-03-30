#pragma once
#include "defs.h"

namespace Tapioca {
class Game;
class WindowManager;
class InputManager;
class FactoryManager;
class SceneManager;
class GraphicsManager;
class PhysicsManager;
class UIManager;
class SoundManager;

Game* game;
WindowManager* window;
InputManager* input;
FactoryManager* factories;
SceneManager* scenes;
GraphicsManager* graphics;
PhysicsManager* physics;
UIManager* ui;
SoundManager* sound;

TAPIOCA_API void initEngine();
TAPIOCA_API void deleteEngine();
TAPIOCA_API void runEngine();

static void createModules(Game* game);
static void createEngineBuilders();
void mapInput();
}
