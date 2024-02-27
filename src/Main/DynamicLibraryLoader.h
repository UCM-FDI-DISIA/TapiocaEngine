#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;
typedef void(__cdecl* EntryPoint)(const char*);

namespace Tapioca {
class DynamicLibraryLoader {
private:
    std::string gameName;
    std::string gamePath;
    HMODULE module;
    EntryPoint entryPoint;

    void free();

public:
    DynamicLibraryLoader(const std::string& gameName = "JuegoPrueba");
    ~DynamicLibraryLoader();

    bool setup();
};
}
