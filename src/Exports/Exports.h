#pragma once
#include "defs.h"

namespace Tapioca {
class MainLoop;
class WindowManager;
class InputManager;
class FactoryManager;
class SceneLoader;
class GraphicsManager;
class PhysicsManager;
class UIManager;
class SoundManager;
class LuaManager;

MainLoop* mainLoop;
WindowManager* window;
InputManager* input;
FactoryManager* factories;
SceneLoader* scenes;
GraphicsManager* graphics;
PhysicsManager* physics;
UIManager* ui;
SoundManager* sound;
LuaManager* lua;

TAPIOCA_API void initEngine();
TAPIOCA_API void deleteEngine();
TAPIOCA_API void runEngine();

static void createModules(MainLoop* mainLoop);
static void createEngineBuilders();
void mapInput();
}
