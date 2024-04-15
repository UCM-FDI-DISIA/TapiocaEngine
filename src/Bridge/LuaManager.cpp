#include "LuaManager.h"
#include <lua.hpp>
#include "LuaRegistry.h"

namespace Tapioca {
LuaManager::LuaManager() : L(nullptr) {
	L = luaL_newstate();
    reg = new LuaRegistry();
}

LuaManager::~LuaManager() {
    delete reg;
	lua_close(L);
}


bool LuaManager::init() {
	return true;
}

}