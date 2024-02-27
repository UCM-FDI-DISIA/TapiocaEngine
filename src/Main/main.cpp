#pragma once
#include <iostream>
#include <string>
#include "Utilities/checkML.h"
#include "DynamicLibraryLoader.h"
#include "Structure/Game.h"
#include "GraphicsEngine.h"
#include "PhysicsManager.h"
#include "InputManager.h"
// #include "AudioManager.h" A�adir cuando se implemente
// #include "UIManager.h" A�adir cuando se implemente
// #include "SceneManager.h"

#include "Node.h" // SOLO PARA PRUEBA
#include "Structure/Scene.h"

using namespace std;
using namespace Tapioca;

static void initModules();
GraphicsEngine* graphics;
PhysicsManager* physics;
InputManager* input;
//AudioManager* audio;
//UIManager* ui;
//SceneManager* scenes;

int main(int argc, char** argv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    DynamicLibraryLoader* loader = new DynamicLibraryLoader();

    if (loader->setup()) {
        Game* game = new Game();
        initModules();
        game->init();
        new Scene(); // Para que no de problemas ahora mismo
        // Cargar desde JSON (LUA) la escena desde el SceneManager y que cree
        // los objetos con sus respectivos componentes y los añada a la escena

        graphics->testScene(); // SOLO PARA PRUEBA
        auto node = graphics->createNode();
        auto childNode = graphics->createChildNode(node);
        graphics->createChildNode(childNode);
        graphics->createChildNode(node);
        graphics->removeNode(node);
        graphics->testScene();
        game->run();
        delete game;
    } else {
        cout << "Error al cargar la librer�a din�mica\n";
    }

    delete loader;

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    return 0;
}

static void initModules() {
    graphics = GraphicsEngine::create();
    physics = PhysicsManager::create();
    input = InputManager::create();
    // audio = AudioManager::create();
    // ui = UIManager::create();
    // scenes = SceneManager::create();
}
