#pragma once
#include <string>
#include <unordered_map>
#include <variant>

using CompValue = std::variant<char, int, float, bool, std::string, std::nullptr_t>;
using CompMap = std::unordered_map<std::string, CompValue>;
typedef void(__cdecl* EntryPointInit)();
typedef std::string(__cdecl* EntryPointGetWindowName)();
typedef std::string(__cdecl* EntryPointGetInitScene)();
