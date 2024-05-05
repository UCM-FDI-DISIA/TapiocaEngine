#include "LuaComponent.h"
#include "../LuaManager.h"
#include <lua.hpp>
#include <LuaBridge.h>
#include <UnorderedMap.h>
#include "../VariantStack.h"

namespace Tapioca {
LuaComponent::LuaComponent(luabridge::LuaRef* objTable, const std::string& name) : objectTable(objTable), name(name) {
    registerFunctions();
}

LuaComponent::~LuaComponent() {
    if (objectTable != nullptr) delete objectTable;
    objectTable = nullptr;
}

void LuaComponent::callSimpleFunction(const std::string& functionName) {
    luabridge::LuaResult result = (*objectTable)[functionName]((*objectTable));
    if (result.hasFailed()) {
        logError(("LuaComponent " + name + ": Ha ocurrido un error durante " + functionName + " [" +
                  std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
                     .c_str());
    }
}

void LuaComponent::registerFunctions() {
    luabridge::getGlobalNamespace(LuaManager::instance()->getLuaState())
        .beginNamespace("_internal")
        .addVariable("comp", *objectTable)
        .endNamespace();

    luabridge::getGlobalNamespace(LuaManager::instance()->getLuaState())
        .beginNamespace("_internal")
        .beginNamespace("comp")
        .addVariable("alive", &alive)
        .addVariable("active", &active)
        .addFunction("pushEvent",
                     [&](const luabridge::LuaRef& comp, const std::string& id, bool global = true, bool delay = false) {
                         pushEvent(id, nullptr, global, delay);
                     })
        .addVariable("object", &object)
        .addVariable("component", this)
        .endNamespace()
        .endNamespace();
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

void LuaComponent::awake() {
    registerFunctions();
    callSimpleFunction("awake");
}

void LuaComponent::start() { callSimpleFunction("start"); }

void LuaComponent::update(const uint64_t deltaTime) {
    luabridge::LuaResult result = (*objectTable)["update"]((*objectTable), deltaTime);
    if (result.hasFailed()) {
        logError(("LuaComponent " + name + ": Ha ocurrido un error durante update [" +
                  std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
                     .c_str());
    }
}

void LuaComponent::fixedUpdate() { callSimpleFunction("fixedUpdate"); }

void LuaComponent::handleEvent(std::string const& id, void* info) {
    luabridge::LuaResult result = (*objectTable)["handleEvent"]((*objectTable), id);
    if (result.hasFailed()) {
        logError(("LuaComponent " + name + ": Ha ocurrido un error durante handleEvent [" +
                  std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
                     .c_str());
    }
}

std::vector<CompValue> LuaComponent::callFunction(const std::string& name, const std::vector<CompValue>& parameters,
                                                  bool* success) const {
    std::vector<CompValue> out;
    luabridge::LuaRef function = luabridge::getGlobal(LuaManager::instance()->getLuaState(), "_internal")["call"];
    if (!function.isCallable()) {
        if (success != nullptr) *success = false;
        return out;
    }
    luabridge::LuaResult result = function(*objectTable, name, parameters);
    if (result.hasFailed()) {
        logError(("LuaManager: Error al ejecutar la funcion de Lua \"" + name + "\" [" +
                  std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
                     .c_str());
        if (success != nullptr) *success = false;
        return out;
    }
    for (int i = 0; i < result.size(); i++) {
        luabridge::LuaRef param = result[0];
        luabridge::TypeResult<CompValue> safeValue = param.cast<CompValue>();
        if (!safeValue) {
            out.push_back(nullptr);
        }
        else {
            out.push_back(safeValue.value());
        }
    }
    if (success != nullptr) *success = true;
    return out;
}

LuaComponentBuilder::LuaComponentBuilder(const std::string& name, luabridge::LuaRef* table)
    : ComponentBuilder(name), classTable(table) { }

LuaComponentBuilder::~LuaComponentBuilder() {
    if (classTable != nullptr) delete classTable;
    classTable = nullptr;
}

Component* LuaComponentBuilder::createComponent() {
    lua_State* L = LuaManager::instance()->getLuaState();
    luabridge::LuaRef* aux = new luabridge::LuaRef(L);
    luabridge::LuaResult result = (*classTable)["new"]((*classTable));
    if (result.hasFailed() || result.size() == 0) {
        logError(("LuaComponent " + id + ": Ha ocurrido un error durante createComponent [" +
                  std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
                     .c_str());
        return nullptr;
    }
    *aux = result[0];
    return new LuaComponent(aux, id);
}
}
