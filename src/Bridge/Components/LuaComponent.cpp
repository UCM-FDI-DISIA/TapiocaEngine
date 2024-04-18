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
    luabridge::LuaResult result = (*objectTable)["initComponent"]((*objectTable), variables);
    if (result.hasFailed()) {
        logError(("LuaComponent " + name + ": Ha ocurrido un error durante initComponent [" +
            std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
            .c_str());
    }
    return result.wasOk() && (result.size() == 0 || result[0]);
}

void LuaComponent::awake() { }

void LuaComponent::callSimpleFunction(std::string functionName) {
    luabridge::LuaResult result = (*objectTable)[functionName]((*objectTable));
    if (result.hasFailed()) {
        logError(("LuaComponent " + name + ": Ha ocurrido un error durante " + functionName + " [" +
                  std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
                     .c_str());
    }
}

void LuaComponent::start() {
    callSimpleFunction("start");
}

void LuaComponent::update(const uint64_t deltaTime) {
    luabridge::LuaResult result = (*objectTable)["update"]((*objectTable), deltaTime);
    if (result.hasFailed()) {
        logError(("LuaComponent " + name + ": Ha ocurrido un error durante update [" +
                  std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
                     .c_str());
    }
}

void LuaComponent::fixedUpdate() {
    callSimpleFunction("fixedUpdate");
}

void LuaComponent::handleEvent(std::string const& id, void* info) {
    luabridge::LuaResult result = (*objectTable)["handleEvent"]((*objectTable), id);
    if (result.hasFailed()) {
        logError(("LuaComponent " + name + ": Ha ocurrido un error durante handleEvent [" +
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