#pragma once
#include <iostream>
#include "Utilities/checkML.h"
#include "DynamicLibraryLoader.h"
#include "Structure/Game.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GraphicsEngine.h"
#include "PhysicsManager.h"
#include "Structure/FactoryManager.h"
// #include "AudioManager.h" A�adir cuando se implemente
// #include "UIManager.h" A�adir cuando se implemente
#include "CreateBuilders.h"
//#include "Utilities/defs.h"
using namespace std;
using namespace Tapioca;

static void createModules(HMODULE module);
InputManager* input;
SceneManager* scenes;
GraphicsEngine* graphics;
PhysicsManager* physics;
FactoryManager* factories;
//AudioManager* audio;
//UIManager* ui;
static void createBuilders(HMODULE module);

int main(int argc, char** argv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    DynamicLibraryLoader* loader = new DynamicLibraryLoader();

    if (loader->load()) {
        Game* game = new Game();
        createModules(loader->getModule());
        createEngineBuilders();
        createBuilders(loader->getModule());
        if (game->init()) {
            //* Prueba
            graphics->createMainCamera();
            graphics->setBackgroundColor(Vector3(0.83f, 0.5f, 0.9f));
            graphics->createLightDirectional(Vector3(0.0f, -1.0f, -1.0f));
            //Node* node = graphics->createNode();
            //graphics->createMesh(node, "mapache.mesh");
            //*/
            game->run();
        } else {
            cerr << "Error al inicializar un módulo\n";
        }
        delete game;
    } else {
        cerr << "Error al cargar la libreria dinamica\n";
    }
    delete loader;

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    return 0;
}

static void createModules(HMODULE module) {
    graphics = GraphicsEngine::create();
    input = InputManager::create();
    factories = FactoryManager::create();
    scenes = SceneManager::create(module);
    physics = PhysicsManager::create();
    // audio = AudioManager::create();
    // ui = UIManager::create();
}
static void createBuilders(HMODULE module) {
    // TODO: Pasar esto a Bridge
    FactoryManager* manager = FactoryManager::instance();

    EntryPoint eP = (EntryPoint)GetProcAddress(module, "getComponentFactories");

    int numFactories;
    FactoryInfo** fI = eP(numFactories);
    for (int i = 0; i < numFactories; ++i) {
        manager->addFactory(fI[i]->name, fI[i]->builder);
        delete fI[i];
    }
    delete[] fI;
}
