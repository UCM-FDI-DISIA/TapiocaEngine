#pragma once
#include <vector>
#include "Structure/ComponentFactory.h"
#include <string>
#include <unordered_map>
#include <variant>
#include <Windows.h>

struct FactoryInfo {
    const char* name;
    Tapioca::ComponentFactory* factory;
};

using CompValue = std::variant<char, int, float, bool, std::string>;
using CompMap = std::unordered_map<std::string, CompValue>;
typedef FactoryInfo**(__cdecl* EntryPoint)(int&);
