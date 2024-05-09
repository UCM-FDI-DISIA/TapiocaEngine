#pragma once
#include "defs.h"

namespace Tapioca {
class MainLoop;
class DynamicLibraryLoader;
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

/**
* @brief Inicializa la referencia al MainLoop y crea los modulos del motor
*/
TAPIOCA_API void initEngine();
/**
* @brief Elimina la referencia al MainLoop
*/
TAPIOCA_API void deleteEngine();
/**
* @brief Intenta inicializar los modulos, crea las factorias, registra las funciones de Lua,
* mapea las entradas si existe el archivo de configuracion e inicializa el bucle principal
* @param gameName Nombre del juego a cargar
*/
TAPIOCA_API void runEngine(std::string const& gameName = "game");
/**
* @brief Crea los modulos y los registra en el MainLoop
* @param mainLoop Referencia al MainLoop
*/
static void createModules(MainLoop* mainLoop);
/**
* @brief Crea las factorias y las registra en el FactoryManager
*/
static void createEngineBuilders();
/**
* @brief Registra las funciones de Lua en el LuaManager
*/
static void registerLuaFunctions();
/**
* @brief Mapea las entradas si existe el archivo de configuracion
*/
void mapInput();
}
