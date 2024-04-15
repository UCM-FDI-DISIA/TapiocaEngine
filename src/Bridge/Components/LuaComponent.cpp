#include "LuaComponent.h"
#include "../LuaManager.h"
#include <lua.hpp>
#include <LuaBridge.h>
#include <UnorderedMap.h>
#include "../VariantStack.h"

namespace Tapioca {
LuaComponent::LuaComponent(luabridge::LuaRef* objTable) : objectTable(objTable) {

}

LuaComponent::~LuaComponent() {
	delete objectTable;
}

bool LuaComponent::initComponent(const CompMap& variables) {
    lua_State* L = LuaManager::instance()->getLuaState();
    // TODO: Revisar como se ve variables en Lua
    luabridge::LuaResult result = (*objectTable)["initComponent"]((*objectTable), variables);
    if (result.hasFailed()) {
        logError(("LuaComponent " + name + ": Ha ocurrido un error durante initComponent [" +
            std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
            .c_str());
    }
    return result.wasOk();
}

void LuaComponent::start() {
    luabridge::LuaResult result = (*objectTable)["start"]((*objectTable));
    if (result.hasFailed()) {
        logError(("LuaComponent " + name + ": Ha ocurrido un error durante start [" +
            std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
            .c_str());
    }
}

LuaComponentBuilder::LuaComponentBuilder(std::string name, luabridge::LuaRef* table)
    : ComponentBuilder(name.c_str()), classTable(table) {
	
}

LuaComponentBuilder::~LuaComponentBuilder() {
    delete classTable;
}

Component* LuaComponentBuilder::createComponent() {
    lua_State* L = LuaManager::instance()->getLuaState();
    luabridge::LuaRef* aux = new luabridge::LuaRef(L);
    luabridge::LuaResult result = (*classTable)["new"]((*classTable));
    if (result.hasFailed() || result.size() == 0) {
        logError(("LuaComponent " + std::string(id) + ": Ha ocurrido un error durante createComponent [" +
            std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
            .c_str());
    }
    *aux = result[0];
	return new LuaComponent(aux);
}
}