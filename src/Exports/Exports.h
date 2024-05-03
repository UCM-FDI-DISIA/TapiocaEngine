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

MainLoop* mainLoop;          // Referencia a la instancia de MainLoop
WindowManager* window;       // Referencia a la instancia de WindowManager
InputManager* input;         // Referencia a la instancia de InputManager
FactoryManager* factories;   // Referencia a la instancia de FactoryManager
SceneLoader* scenes;         // Referencia a la instancia de SceneLoader
GraphicsManager* graphics;   // Referencia a la instancia de GraphicsManager
PhysicsManager* physics;     // Referencia a la instancia de PhysicsManager
UIManager* ui;               // Referencia a la instancia de UIManager
SoundManager* sound;         // Referencia a la instancia de SoundManager
LuaManager* lua;             // Referencia a la instancia de LuaManager

/*
* Inicializa la referencia al MainLoop y crea los modulos del motor
*/
TAPIOCA_API void initEngine();
/*
* Elimina la referencia al MainLoop
*/
TAPIOCA_API void deleteEngine();
/*
* Intenta inicializar los modulos, crea las factorias, registra las funciones de Lua
* mapea las entradas si existe el archivo de configuracion e inicializa el bucle principal
*/
TAPIOCA_API void runEngine();
/*
* Crea los modulos y los registra en el MainLoop
*/
static void createModules(MainLoop* mainLoop);
/*
* Crea las factorias y las registra en el FactoryManager
*/
static void createEngineBuilders();
/*
* Registra las funciones de Lua en el LuaManager
*/
static void registerLuaFunctions();
/*
* Mapea las entradas si existe el archivo de configuracion
*/
void mapInput();
}
