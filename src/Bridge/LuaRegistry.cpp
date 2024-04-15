#include "LuaRegistry.h"
#include "LuaManager.h"

namespace Tapioca {
LuaRegistry::LuaRegistry() {
	L = LuaManager::instance()->getLuaState();
    //luabridge::
}
}