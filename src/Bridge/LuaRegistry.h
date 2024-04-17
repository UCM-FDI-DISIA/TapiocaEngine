#pragma once
#include "defs.h"
#include <lua.hpp>
#include <LuaBridge.h>

struct lua_State;

namespace Tapioca {
    // TODO: Revisar (pero mas importante)
class TAPIOCA_API LuaRegistry { 
private:
    lua_State* L;
    luabridge::LuaRef map;

public:
    LuaRegistry(lua_State* L);

    template<typename TFunc>
    void registerFunction(std::string className, std::string functionName, TFunc function) {
        luabridge::getGlobalNamespace(L)
            .beginNamespace("Tapioca")
            .beginNamespace(className.c_str())
            .addFunction(functionName, function)
            .endNamespace()
            .endNamespace();
            
    }

    template<typename T>
    T getValueFromLua(std::string name) {
        return map[name].cast<T>();
    }
};
}