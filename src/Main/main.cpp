#pragma once
#include <iostream>
#include "Utilities/checkML.h"
#include "DynamicLibraryLoader.h"
#include "Structure/Game.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GraphicsEngine.h"
#include "PhysicsManager.h"
// #include "AudioManager.h" A�adir cuando se implemente
// #include "UIManager.h" A�adir cuando se implemente
using namespace std;
using namespace Tapioca;

static void createModules(HMODULE module);
InputManager* input;
SceneManager* scenes;
GraphicsEngine* graphics;
PhysicsManager* physics;
//AudioManager* audio;
//UIManager* ui;

int main(int argc, char** argv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    DynamicLibraryLoader* loader = new DynamicLibraryLoader();

    if (loader->load()) {
        Game* game = new Game();
        createModules(loader->getModule());
        if (game->init()) {
            auto node = graphics->createNode();
            auto childNode = graphics->createChildNode(node);
            graphics->createChildNode(childNode);
            graphics->createChildNode(node);
            graphics->removeNode(node);
            graphics->testScene();
            game->run();
        } else {
            cerr << "Error al inicializar un módulo\n";
        }
        delete game;
    } else {
        cerr << "Error al cargar la librer�a din�mica\n";
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
