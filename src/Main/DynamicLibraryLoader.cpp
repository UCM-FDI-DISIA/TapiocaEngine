#include "DynamicLibraryLoader.h"

namespace Tapioca {
DynamicLibraryLoader::DynamicLibraryLoader() : module(nullptr), entryPoint(nullptr) {
    //gameName = UI();
    gameName = "MiBiblioteca";
}

DynamicLibraryLoader::~DynamicLibraryLoader() { free(); }

string DynamicLibraryLoader::UI() {
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

bool DynamicLibraryLoader::setup() {
#ifdef _DEBUG
    gamePath = "./" + gameName + "_d.dll";
#else
    gamePath = "./" + gameName + ".dll";
#endif

    std::cout << "Cargando " << gamePath << "...\n";

    if ((module = LoadLibraryA(gamePath.c_str())) == nullptr) {
        std::cerr << "Error al cargar la DLL.\n";
        return false;
    }

    if ((entryPoint = (EntryPoint)(GetProcAddress(module, "saluda"))) == nullptr) {
        std::cerr << "No se pudo obtener la dirección de la función.\n";
        free();
        return false;
    }

    entryPoint("mundo");   // Hola, mundo

    return true;
}

void DynamicLibraryLoader::free() {
    if (module != nullptr) {
        FreeLibrary(module);
    }
}
}