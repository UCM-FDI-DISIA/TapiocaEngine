#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;

namespace Tapioca {
class DynamicLibraryLoader {
private:
    string gameName;
    string gamePath;
    HMODULE module;

    void free();

public:
    DynamicLibraryLoader(const string& gameName = "JuegoPrueba");
    ~DynamicLibraryLoader();

    inline HMODULE getModule() const { return module; }

    bool load();
};
}
