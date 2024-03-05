#pragma once
#include <string>
#include <unordered_map>
#include <variant>

#define TAPIOCA_API __declspec(dllexport)

#ifdef _DEBUG
#include <iostream>
#endif;

namespace Tapioca {
class FactoryManager;
}

using CompValue = std::variant<char, int, float, bool, std::string>;
using CompMap = std::unordered_map<std::string, CompValue>;
typedef void(__cdecl* EntryPoint)(Tapioca::FactoryManager*);
