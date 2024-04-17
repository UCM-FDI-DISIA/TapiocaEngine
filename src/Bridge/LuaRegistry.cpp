#include "LuaRegistry.h"
#include "LuaManager.h"

namespace Tapioca {
LuaRegistry::LuaRegistry(lua_State* L) : L(L), map(L) {
    map = luabridge::newTable(L);
    luabridge::getGlobalNamespace(L).beginNamespace("_internal").addVariable("export", map).endNamespace();
    luaL_dostring(L, "export = _internal.export");
}
}