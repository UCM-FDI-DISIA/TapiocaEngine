#pragma once
#include <iostream>
#include <string>
#include "Utilities/checkML.h"
#include "DynamicLibraryLoader.h"
#include "Structure/Game.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GraphicsEngine.h"
#include "PhysicsManager.h"
// #include "AudioManager.h" A�adir cuando se implemente
// #include "UIManager.h" A�adir cuando se implemente

#include "Node.h" // SOLO PARA PRUEBA
#include "Structure/Scene.h"

using namespace std;
using namespace Tapioca;

static void createModules(HMODULE module);
GraphicsEngine* graphics;
PhysicsManager* physics;
InputManager* input;
//AudioManager* audio;
//UIManager* ui;
SceneManager* scenes;

int main(int argc, char** argv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    DynamicLibraryLoader* loader = new DynamicLibraryLoader();

    if (loader->load()) {
        Game* game = new Game();
        createModules(loader->getModule());
        game->init();
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

static void createModules(HMODULE module) {
    input = InputManager::create();
    scenes = SceneManager::create(module);
    graphics = GraphicsEngine::create();
    physics = PhysicsManager::create();
    // audio = AudioManager::create();
    // ui = UIManager::create();
}
