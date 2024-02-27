#include "DynamicLibraryLoader.h"

namespace Tapioca {
DynamicLibraryLoader::DynamicLibraryLoader(const string& gameName) : 
    gameName(gameName), module(nullptr) { }

DynamicLibraryLoader::~DynamicLibraryLoader() { free(); }

bool DynamicLibraryLoader::load() {
#ifdef _DEBUG
    gamePath = "./" + gameName + "_d.dll";
#else
    gamePath = "./" + gameName + ".dll";
#endif

#ifdef _DEBUG
    cout << "Cargando " << gamePath << "...\n";
#endif

    if ((module = LoadLibraryA(gamePath.c_str())) == nullptr) {
        cerr << "Error al cargar la DLL.\n";
        return false;
    }
    return true;
}

void DynamicLibraryLoader::free() {
    if (module != nullptr) {
        FreeLibrary(module);
    }
}
}