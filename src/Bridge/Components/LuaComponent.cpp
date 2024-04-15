#include "LuaComponent.h"
#include "../LuaManager.h"
#include <lua.hpp>
#include <LuaBridge.h>

namespace Tapioca {
LuaComponent::LuaComponent(luabridge::LuaRef* objTable) : objectTable(objTable) {

}

LuaComponent::~LuaComponent() {
	delete objectTable;
}

bool LuaComponent::initComponent(const CompMap& variables) {
    (*objectTable)["variables"] = luabridge::newTable(LuaManager::instance()->getLuaState());

    for (auto variable : variables) {
        (*objectTable)["variables"][variable.first] = variable.second;
    }

    luabridge::LuaResult result = (*objectTable)["initComponent"]((*objectTable),variables);
    if (result.hasFailed()) {
        logError(("LuaComponent " + name + ": Ha ocurrido un error durante initComponent [" +
                  std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
                     .c_str());
    }
    return result.wasOk();
}

LuaComponentBuilder::LuaComponentBuilder(std::string name) : ComponentBuilder(name.c_str()) {
	
}

LuaComponentBuilder::~LuaComponentBuilder() {

}

Component* LuaComponentBuilder::createComponent() {
    luabridge::LuaRef* aux = new luabridge::LuaRef(LuaManager::instance()->getLuaState());
    luabridge::LuaResult result = (*classTable)();
    if (result.hasFailed() || result.size() == 0) {
        logError("Oops");
    }
    *aux = result[0];
	return new LuaComponent(aux);
}
}