#pragma once
#include <string>
#include <unordered_map>
#include <variant>

using CompValue = std::variant<char, int, float, bool, std::string>;
using CompMap = std::unordered_map<std::string, CompValue>;
struct Function {
    const char* functionName;
    void (*function)();
};
typedef void(__cdecl* EntryPointInit)();
typedef std::string(__cdecl* EntryPointGetWindowName)();
typedef std::string(__cdecl* EntryPointGetInitScene)();
typedef int(__cdecl* EntryPointGetFunctions)(Function*,int);
