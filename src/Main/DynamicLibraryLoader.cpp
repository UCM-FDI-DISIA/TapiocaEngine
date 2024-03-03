#include "DynamicLibraryLoader.h"
#include <iostream>

namespace Tapioca {
DynamicLibraryLoader::DynamicLibraryLoader(const std::string& gameName) : gameName(gameName), module(nullptr) { }

DynamicLibraryLoader::~DynamicLibraryLoader() { freeModule(); }

bool DynamicLibraryLoader::load() {
#ifdef _DEBUG
    gamePath = "./" + gameName + "_d.dll";
#else
    gamePath = "./" + gameName + ".dll";
#endif

#ifdef _DEBUG
    std::cout << "Cargando " << gamePath << "...\n";
#endif

    if ((module = LoadLibraryA(gamePath.c_str())) == nullptr) {
        std::cerr << "Error al cargar la DLL.\n";
        return false;
    }
    return true;
}

void DynamicLibraryLoader::freeModule() {
    if (module != nullptr) {
        FreeLibrary(module);
    }
}
}