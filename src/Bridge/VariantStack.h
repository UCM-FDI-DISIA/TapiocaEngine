#pragma once
#include <detail/Stack.h>
#include "componentDefs.h"

namespace luabridge {
template<>
struct Stack<CompValue> {
    [[nodiscard]] static Result push(lua_State* L, CompValue value) {
#if LUABRIDGE_SAFE_STACK_CHECKS
        if (!lua_checkstack(L, 1)) return makeErrorCode(ErrorCode::LuaStackOverflow);
#endif
        switch (value.index()) {
        case 0: return Stack<char>::push(L, std::get<char>(value));
        case 1: return Stack<int>::push(L, std::get<int>(value));
        case 2: return Stack<float>::push(L, std::get<float>(value));
        case 3: return Stack<bool>::push(L, std::get<bool>(value));
        case 4: return Stack<std::string>::push(L, std::get<std::string>(value));
        default: break;
        }
        // TODO: Enviar error si se llega aquí
        return {};
    }

    [[nodiscard]] static TypeResult<CompValue> get(lua_State* L, int index) {
        if (lua_isboolean(L, index)) {
            return lua_toboolean(L, index) == 1;
        }
        else if (lua_isinteger(L, index)) {
            return (int)lua_tointeger(L, index);
        }
        else if (lua_isnumber(L, index)) {
            return (float)lua_tonumber(L, index);
        }
        else if (lua_isstring(L, index)) {
            return lua_tostring(L, index);
        }
        // TODO: Enviar error si se llega aquí
    }

    [[nodiscard]] static bool isInstance(lua_State* L, int index) { return lua_isstring(L, index); }
};
}