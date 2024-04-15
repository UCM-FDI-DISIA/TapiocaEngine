#pragma once
#include "defs.h"
#include <LuaBridge.h>
#include <lua.hpp>

struct lua_State;

namespace Tapioca {
class TAPIOCA_API LuaRegistry { 
private:
    lua_State* L;

public:
    LuaRegistry();

    template<typename TClass, typename TFunc>
    void registerFunction(std::string className, std::string functionName, TFunc function) {
        luabridge::getGlobalNamespace(L)
            .beginClass<TClass>(className)
            .addFunction(functionName, function)
            .endClass();
    }
};
}