#pragma once
#include <string>
#include <unordered_map>
#include <variant>

using CompValue = std::variant<char, int, float, bool, std::string>;
using CompMap = std::unordered_map<std::string, CompValue>;
typedef void(__cdecl* EntryPoint)();