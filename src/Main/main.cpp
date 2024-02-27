#pragma once
#include <iostream>
#include <string>
#include "Utilities/checkML.h"
#include "DynamicLibraryLoader.h"
#include "Structure/Game.h"
#include "GraphicsEngine.h"
#include "PhysicsManager.h"
#include "lua1.h"
// #include "InputManager.h"
// #include "AudioManager.h" Añadir cuando se implemente
// #include "UIManager.h" Añadir cuando se implemente

//#include "Node.h" // SOLO PARA PRUEBA

using namespace std;
using namespace Tapioca;

static void initModules();
GraphicsEngine* graphics;
PhysicsManager* physics;
//InputManager* input;
//AudioManager* audio;
//UIManager* ui;

int main(int argc, char** argv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    lua1 aaa;
    aaa.Prueba();

    //DynamicLibraryLoader* loader = new DynamicLibraryLoader();

    //if (loader->setup()) {
    //    Game* game = new Game();
    //    initModules();
    //    game->init();
    //    /*auto node = graphics->createNode();
    //    auto childNode = graphics->createChildNode(node);
    //    graphics->createChildNode(childNode);
    //    graphics->createChildNode(node);
    //    graphics->removeNode(childNode);
    //    node->getAllChildren();*/
    //    graphics->testScene();
    //    game->run();
    //    delete game;
    //} else {
    //    cout << "Error al cargar la librería dinámica\n";
    //}

    //delete loader;

    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    //_CrtDumpMemoryLeaks();
    return 0;
}

static void initModules() {
    graphics = GraphicsEngine::create();
    physics = PhysicsManager::create();
    // input = InputManager::create();
    // audio = AudioManager::create();
    // ui = UIManager::create();
}
