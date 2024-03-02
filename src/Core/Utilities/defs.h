#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>
#include <Windows.h>
#include "Structure/ComponentBuilder.h"
using namespace std;

struct FactoryInfo {
    const char* name;
    Tapioca::ComponentBuilder* builder;
};

using CompValue = variant<char, int, float, bool, string>;
using CompMap = unordered_map<string, CompValue>;
typedef FactoryInfo**(__cdecl* EntryPoint)(int&);
