#include "DynamicLibraryLoader.h"
#include "componentDefs.h"
#include "defs.h"
#include "FactoryManager.h"

namespace Tapioca {
HMODULE DynamicLibraryLoader::module = nullptr;

bool DynamicLibraryLoader::load(std::string const& gameName) {
    std::string gamePath;   // Ruta del juego con formato
#ifdef _DEBUG
    gamePath = "./" + gameName + "_d.dll";
#else
    gamePath = "./" + gameName + ".dll";
#endif

#ifdef _DEBUG
    std::cout << "Cargando " << gamePath << "...\n";
#endif

    if ((module = LoadLibraryA(gamePath.c_str())) == nullptr) {
#ifdef _DEBUG
        std::cerr << "Error al cargar la DLL.\n";
#endif
        return false;
    }

#ifdef _DEBUG
    std::cout << "Se ha cargado \"" << gameName << "\" correctamente\n";
#endif

    return true;
}

bool DynamicLibraryLoader::initGame(std::string const& gameName) {
    if (load(gameName)) {
        EntryPoint eP = (EntryPoint)GetProcAddress(module, "init");
        if (eP == nullptr) {
#ifdef _DEBUG
            std::cerr << "La DLL del juego no tiene la funcion \"init\"\n";
#endif
            return false;
        }
        return eP();
    }
    else {
#ifdef _DEBUG
        std::cerr << "Error al cargar la DLL del juego\n";
#endif
        return false;
    }
}

void DynamicLibraryLoader::freeModule() {
    if (module != nullptr) {
        FreeLibrary(module);
    }
}
}
