#include "DynamicLibraryLoader.h"

#ifdef _DEBUG
#include <iostream>
#endif

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

void DynamicLibraryLoader::freeModule() {
    if (module != nullptr) {
        FreeLibrary(module);
    }
}
}