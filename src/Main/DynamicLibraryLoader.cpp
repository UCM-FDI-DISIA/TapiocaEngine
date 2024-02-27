#include "DynamicLibraryLoader.h"

namespace Tapioca {
DynamicLibraryLoader::DynamicLibraryLoader(const std::string& gameName) : 
    gameName(gameName), module(nullptr), entryPoint(nullptr) { }

DynamicLibraryLoader::~DynamicLibraryLoader() { free(); }

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