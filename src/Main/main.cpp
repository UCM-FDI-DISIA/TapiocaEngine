#pragma once
#include "Exports.h"
#include "checkML.h"

#ifdef _DEBUG
#include <iostream>

int main(int argc, char** argv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

    Tapioca::initEngine();
    // Se puede pasar un argumento para cargar un juego en concreto
    if (argc > 1) Tapioca::runEngine(argv[1]);
    // Si no se pasa ningún argumento, se carga el juego "game" por defecto
    else Tapioca::runEngine();
    Tapioca::deleteEngine();

    return 0;
}

#else
#include <Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    Tapioca::initEngine();
    if (pCmdLine && wcslen(pCmdLine) > 0) {
        std::wstring ws(pCmdLine);
        std::string commandLine(ws.begin(), ws.end());
        Tapioca::runEngine(commandLine.c_str());
    }
    else Tapioca::runEngine();
    Tapioca::deleteEngine();

    return 0;
}
#endif
