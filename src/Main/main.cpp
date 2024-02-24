#pragma once
#include <iostream>
#include <string>
#include "checkML.h"
#include "Structure/Game.h"
using namespace std;
using namespace Tapioca;

static string UI();

int main(int argc, char** argv) {

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);   // Check Memory Leaks

    cout << "Tapioca Engine\n";

    string gameName = UI();

    Game* game = new Game(gameName);

    if (!game->setup()) {
        std::cerr << "Error al hacer setup de la biblioteca.\n";
        delete game;
        return -1;
    }

    // Otras cosas

    delete game;
    return 0;
}

static string UI() {

    cout << "Elige el juego a cargar: \n\t1. Billiards adrift\n\t2. Mar.io\n\t3. Otro [INPUT]\n> ";

    char opt = ' ';
    cin >> opt;

    string gameName = "";
    switch (opt) {
    case '1':
        gameName = "Billiards_adrift";
        break;
    case '2':
        gameName = "Mar_io";
        break;
    case '3':
        cout << "Introduce el nombre: \n> ";
        cin >> gameName;
        break;
    default:
        cout << "Por favor, elige un juego.\n";
        return UI();
    }

    return gameName;
}