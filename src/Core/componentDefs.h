#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <variant>

using CompValue = std::variant<char, int, float, bool, std::string>;
using CompMap = std::unordered_map<std::string, CompValue>;
typedef void(__cdecl* EntryPointInit)();
typedef std::string(__cdecl* EntryPointGetWindowName)();
typedef std::string(__cdecl* EntryPointGetInitScene)();
typedef std::vector<std::pair<std::string, std::function<void()>>>(__cdecl* EntryPointGetFunctions)();
