#pragma once
#include "DynamicLibraryLoader.h"
#include "Structure/Game.h"

#include "InputManager.h"
#include "SceneManager.h"
#include "GraphicsEngine.h"
#include "PhysicsManager.h"
// #include "AudioManager.h" Descomentar cuando se implemente
// #include "UIManager.h" Descomentar cuando se implemente

#include "Structure/FactoryManager.h"
#include "CreateBuilders.h"
//#include "Utilities/defs.h"

#include "Utilities/checkML.h"
#include <iostream>


// IMPORTANTE: METERLO EN UN NAMESPACE CAUSA ERRORES DE COMPILACION

Tapioca::InputManager* input;
Tapioca::SceneManager* scenes;
Tapioca::GraphicsEngine* graphics;
Tapioca::PhysicsManager* physics;
Tapioca::FactoryManager* factories;
//Tapioca::AudioManager* audio;
//Tapioca::UIManager* ui;

// TODO: SOLO PARA PRUEBAS, BORRAR
#include "Node.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "Viewport.h"
#include "Utilities/Vector3.h"

static void createModules(HMODULE module);

static void createBuilders(HMODULE module);


int main(int argc, char** argv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Tapioca::DynamicLibraryLoader* loader = new Tapioca::DynamicLibraryLoader();

    if (loader->load()) {
        Tapioca::Game* game = new Tapioca::Game();
        createModules(loader->getModule());
        Tapioca::createEngineBuilders();
        createBuilders(loader->getModule());
        if (game->init()) {
            //* Prueba
            auto nodeCamera = graphics->createNode(Tapioca::Vector3(0.0f, 0.0f, 20.0f));
            auto camera = graphics->createCamera(nodeCamera, "MainCamera");
            auto viewport = graphics->createViewport(camera, 0);
            viewport->setBackground(Tapioca::Vector3(0.925f, 0.698f, 0.941));

            auto node = graphics->createNode();
            auto light = graphics->createLightDirectional(node, Tapioca::Vector3(0.0f, -1.0f, -1.0f));
            auto mesh = graphics->createMesh(node, "racoon/mapache.mesh");

            game->run();

            delete nodeCamera;
            delete camera;
            delete viewport;

            delete light;
            delete node;
            delete mesh;
        }
        else
            std::cerr << "Error al inicializar un modulo\n";

        delete game;
    }
    else
        std::cerr << "Error al cargar la libreria dinamica\n";

    delete loader;

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    return 0;
}


static void createModules(HMODULE module) {
    graphics = Tapioca::GraphicsEngine::create();
    input = Tapioca::InputManager::create();
    factories = Tapioca::FactoryManager::create();
    scenes = Tapioca::SceneManager::create(module);
    physics = Tapioca::PhysicsManager::create();
    // audio = AudioManager::create();
    // ui = UIManager::create();
}


static void createBuilders(HMODULE module) {
    // TODO: Pasar esto a Bridge
    Tapioca::FactoryManager* manager = Tapioca::FactoryManager::instance();

    EntryPoint eP = (EntryPoint)GetProcAddress(module, "getComponentFactories");

    int numFactories;
    FactoryInfo** fI = eP(numFactories);
    for (int i = 0; i < numFactories; ++i) {
        manager->addFactory(fI[i]->name, fI[i]->builder);
        delete fI[i];
    }
    delete[] fI;
}
