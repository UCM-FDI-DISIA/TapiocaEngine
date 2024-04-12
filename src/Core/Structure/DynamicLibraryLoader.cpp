#include "DynamicLibraryLoader.h"
#include "componentDefs.h"
#include "defs.h"
#include "MainLoop.h"

namespace Tapioca {
HMODULE DynamicLibraryLoader::module = nullptr;

bool DynamicLibraryLoader::load(std::string const& gameName) {
    std::string gamePath;   // Ruta del juego con formato
#ifdef _DEBUG
    gamePath = "./" + gameName + "_d.dll";
#else
    gamePath = "./" + gameName + ".dll";
#endif

    logInfo(("DynamicLibraryLoader: Cargando \"" + gamePath + "\"...").c_str());

    if ((module = LoadLibraryA(gamePath.c_str())) == nullptr) {
        logError("DynamicLibraryLoader: Error al cargar la DLL.");
        return false;
    }

    logInfo(("DynamicLibraryLoader: Se ha cargado \"" + gameName + "\" correctamente.").c_str());

    return true;
}

bool DynamicLibraryLoader::initGame(std::string const& gameName) {
    if (load(gameName)) {
        EntryPointInit eP = (EntryPointInit)GetProcAddress(module, "init");
        if (eP == nullptr) {
            logError("DynamicLibraryLoader: La DLL del juego no tiene la funcion \"init\".");
            return false;
        }
        eP();
        return MainLoop::instance()->initConfig();
    }
    else {
        logError("DynamicLibraryLoader: Error al cargar la DLL del juego.");
        return false;
    }
}

void DynamicLibraryLoader::freeModule() {
    if (module != nullptr) {
        FreeLibrary(module);
    }
}
}
