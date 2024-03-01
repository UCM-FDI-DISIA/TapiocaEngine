#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>
#include <Windows.h>
#include "Structure/ComponentFactory.h"
using namespace std;

struct FactoryInfo {
    const char* name;
    Tapioca::ComponentFactory* factory;
};

using CompValue = variant<char, int, float, bool, string>;
using CompMap = unordered_map<string, CompValue>;
typedef FactoryInfo**(__cdecl* EntryPoint)(int&);
