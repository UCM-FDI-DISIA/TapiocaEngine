#pragma once
#include <string>
#include <unordered_map>
#include <variant>

using CompValue = std::variant<char, int, float, bool, std::string, std::nullptr_t>;
using CompMap = std::unordered_map<std::string, CompValue>;
typedef void(__cdecl* EntryPointInit)();
typedef const char*(__cdecl* EntryPointGetWindowName)();
typedef const char*(__cdecl* EntryPointGetInitScene)();
typedef bool(__cdecl* EntryPointGetFullScreen)();
typedef void(__cdecl* EntryPointGetWindowSize)(uint32_t&, uint32_t&);
