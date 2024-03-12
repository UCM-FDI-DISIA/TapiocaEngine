#pragma once
#include "defs.h"

namespace Tapioca {
class Game;
class WindowManager;
class InputManager;
class FactoryManager;
class SceneManager;
class GraphicsEngine;
class PhysicsManager;
class UIManager;
class SoundEngine;

Game* game;
WindowManager* window;
InputManager* input;
FactoryManager* factories;
SceneManager* scenes;
GraphicsEngine* graphics;
PhysicsManager* physics;
UIManager* ui;
SoundEngine* sound;

TAPIOCA_API void initEngine();
TAPIOCA_API void deleteEngine();
TAPIOCA_API void runEngine();

static void createModules(Game* game);
void mapInput();
}
