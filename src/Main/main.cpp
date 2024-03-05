#pragma once
#include "Utilities/checkML.h"
#include "DynamicLibraryLoader.h"
#include "Structure/Game.h"

#include "GraphicsEngine.h"
#include "InputManager.h"
#include "Structure/FactoryManager.h"
#include "SceneManager.h"
#include "PhysicsManager.h"
// #include "AudioManager.h" Descomentar cuando se implemente
#include "UIManager.h"

#include "CreateBuilders.h"

#ifdef _DEBUG
#include <iostream>
#endif

// IMPORTANTE: METERLO EN UN NAMESPACE CAUSA ERRORES DE COMPILACION

Tapioca::InputManager* input;
Tapioca::FactoryManager* factories;
Tapioca::SceneManager* scenes;
Tapioca::GraphicsEngine* graphics;
Tapioca::PhysicsManager* physics;
Tapioca::UIManager* ui;
//Tapioca::AudioManager* audio;

static void createModules(Tapioca::Game*);

// TODO: solo para pruebas, borrar
#include "Node.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "Viewport.h"
#include "Utilities/Vector3.h"

int main(int argc, char** argv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Tapioca::DynamicLibraryLoader* loader = new Tapioca::DynamicLibraryLoader();

    if (loader->load()) {
        Tapioca::Game* game = new Tapioca::Game();
        createModules(game);
        Tapioca::createEngineBuilders();
        Tapioca::createGameBuilders(loader->getModule());
        if (game->init()) {
            //* Prueba
            auto nodeCamera = graphics->createNode(Tapioca::Vector3(0.0f, 0.0f, 20.0f));
            auto camera = graphics->createCamera(nodeCamera, "MainCamera");
            auto viewport = graphics->createViewport(camera, 0);
            viewport->setBackground(Tapioca::Vector3(0.925f, 0.698f, 0.941f));

            auto node = graphics->createNode();
            auto light = graphics->createLightDirectional(node, Tapioca::Vector3(0.0f, -1.0f, -1.0f));

            node->setParent(nodeCamera);
            //*/

            game->run();

            //*
            delete node;

            delete nodeCamera;
            delete camera;
            delete viewport;
            delete light;
            //*/
        }
#ifdef _DEBUG
        else
            std::cerr << "Error al inicializar un modulo\n";
#endif
        delete game;
    }
#ifdef _DEBUG
    else
        std::cerr << "Error al cargar la libreria dinamica\n";
#endif

    delete loader;

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    return 0;
}

static void createModules(Tapioca::Game* game) {
    graphics = Tapioca::GraphicsEngine::create();
    game->addModule(graphics);
    ui = Tapioca::UIManager::create();
    game->addModule(ui);
    input = Tapioca::InputManager::create();
    game->addModule(input);
    factories = Tapioca::FactoryManager::create();
    game->addModule(factories);
    scenes = Tapioca::SceneManager::create();
    game->addModule(scenes);
    physics = Tapioca::PhysicsManager::create();
    game->addModule(physics);
    // audio = AudioManager::create();
    // game->addModule(audio);
}
