#include "LuaRegistry.h"
#include "LuaManager.h"
#include <lua.hpp>
#include <LuaBridge.h>

namespace Tapioca {
LuaRegistry::LuaRegistry() {
	L = LuaManager::instance()->getLuaState();
    //luabridge::
}
}