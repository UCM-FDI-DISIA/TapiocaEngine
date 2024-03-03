#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>
#include <Windows.h>
#include "Structure/ComponentBuilder.h"

struct FactoryInfo {
    const char* name;
    Tapioca::ComponentBuilder* builder;
};

using CompValue = std::variant<char, int, float, bool, std::string>;
using CompMap = std::unordered_map<std::string, CompValue>;
typedef FactoryInfo**(__cdecl* EntryPoint)(int&);
