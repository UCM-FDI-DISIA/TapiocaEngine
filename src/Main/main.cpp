#pragma once
#include "checkML.h"
#include "Exports.h"

int main(int argc, char** argv) {
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    Tapioca::initEngine();
    // Se puede pasar un argumento para cargar un juego en concreto
    if (argc > 1) Tapioca::runEngine(argv[1]);
    // Si no se pasa ningun argumento, se carga el juego "game" por defecto
    else Tapioca::runEngine();
    Tapioca::deleteEngine();

#ifdef _DEBUG
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif
    return 0;
}
