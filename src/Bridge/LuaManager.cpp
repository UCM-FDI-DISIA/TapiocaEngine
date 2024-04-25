#include "LuaManager.h"
#include <lua.hpp>
#include "LuaRegistry.h"
#include <LuaBridge.h>
#include <Vector.h>
#include "VariantStack.h"
#include <filesystem>
#include "Components/LuaComponent.h"
#include "Structure/FactoryManager.h"
#include <sstream>
#include "checkML.h"
#include <functional>
#include "Structure/GameObject.h"
#include "Structure/Scene.h"

namespace Tapioca {

LuaManager* LuaManager::instance_ = nullptr;

LuaManager::LuaManager() : L(nullptr), initialized(true) {
    L = luaL_newstate();
    luaL_openlibs(L);

    luabridge::getGlobalNamespace(L).addFunction("print", &print);

    if (luaL_dofile(L, "internal.lua")!=0) {
        logError(("LuaManager: Error al cargar internal.lua: " + std::string(lua_tostring(L, -1))).c_str());
        initialized = false;
    }

    luabridge::getGlobalNamespace(L)
        .beginNamespace("Tapioca")
        .beginClass<Scene>("Scene")
        .addFunction("getObjects", &Scene::getObjects)
        .addFunction("getHandler", &Scene::getHandler)
        .addProperty("name", &Scene::getName)
        .addProperty("active", &Scene::isActive, &Scene::setActive)
        .endClass()
        .beginClass<GameObject>("GameObject")
        .addFunction("getHandler", &GameObject::getHandler)
        .addProperty("alive", &GameObject::isAlive)
        .addFunction("die", &GameObject::die)
        .addProperty("scene", &GameObject::getScene)
        .addFunction("addComponent",
            +[](GameObject* obj, const std::string& id, const CompMap& variables) -> Component* {
                return obj->addComponent(id, variables);
            })
        .addFunction("addComponents", &GameObject::addComponents)
        .addFunction("getComponent",
            +[](GameObject* obj, const std::string& id) -> Component* {
                return obj->getComponent(id);
            })
        .addFunction("getAllComponents", &GameObject::getAllComponents)
        .addFunction("getComponents",
            +[](GameObject* obj, const std::string& id) -> std::vector<Component*> {
                return obj->getComponents(id);
            })
        .endClass()
        .beginClass<Component>("Component")
        .addFunction("pushEvent",
            +[](Component* comp, const std::string& id, bool global = true, bool delay = false) {
                comp->pushEvent(id, nullptr, global, delay);
            })
        .addProperty("object", &Component::getObject)
        .addProperty("alive", &Component::isAlive)
        .addFunction("die", &Component::die)
        .addProperty("active", &Component::isActive, &Component::setActive)
        .endClass()
        .deriveClass<LuaComponent, Component>("LuaComponent")
        .addProperty("table", [](LuaComponent* comp) -> luabridge::LuaRef* { return comp->getTable(); })
        .endClass()
        .endNamespace();

    reg = new LuaRegistry(L);
}

LuaManager::~LuaManager() {
    delete reg;
    lua_close(L);
}

bool LuaManager::init() {
    return initialized && loadBase() && loadScripts();
}

std::vector<CompValue> LuaManager::callLuaFunction(const std::string& name, const std::vector<CompValue>& parameters, bool* success) {
    std::vector<CompValue> out;
    luabridge::LuaRef function = luabridge::getGlobal(L, "_internal")["callGlobal"];
    if (!function.isCallable()) {
        if(success != nullptr) *success = false;
        return out;
    }
    luabridge::LuaResult result = function(name, parameters);
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

bool LuaManager::addLuaFunction(const std::string& name, std::function<void()> f) {
    luabridge::getGlobalNamespace(L).addFunction(name.c_str(), f);
    return true;
}

CompValue LuaManager::getValueFromLua(const std::string& name) {
    luabridge::LuaRef value = luabridge::getGlobal(L, name.c_str());
    luabridge::TypeResult<CompValue> result = value.cast<CompValue>();
    if (!result) {
        logError("LuaManager: Se ha intentado sacar un valor invalido.");
        return nullptr;
    }
    return result.value();
}

bool LuaManager::setValueOnLua(const std::string& name, CompValue value) {
    bool done = luabridge::setGlobal(L, value, name.c_str());
    if (!done) {
        logError("LuaManager: Se ha intentado poner un nombre invalido.");
    }
    return done;
}

bool LuaManager::loadBase() {
    if (luaL_dofile(L, BASE_FILE) != 0) {
        logError(("LuaManager: Error al cargar base de LuaComponent: " + std::string(lua_tostring(L, -1))).c_str());
        return false;
    }
    return true;
}

bool LuaManager::loadScript(const std::filesystem::path& path) {
    if (!loadBase()) return false;

    if (luaL_dofile(L, path.string().c_str()) != 0) {
        logError(("LuaManager: Error al cargar el archivo Lua: " + std::string(lua_tostring(L, -1))).c_str());
        return false;
    }
    luabridge::LuaRef* table = new luabridge::LuaRef(luabridge::getGlobal(L, "comp"));
    std::string name = path.filename().string();
    // Se le quita la extension (.lua)
    name = name.substr(0, name.length() - 4);
    FactoryManager::instance()->addBuilder(
        new LuaComponentBuilder(name, table));   // Desde 18:40 a 21:15, Desde 22:22 a 00:45
    return true;
}

bool LuaManager::loadScripts() {
    logInfo("LuaManager: Cargando scripts...");
    std::string path = "assets\\scripts\\";
    std::string extension = ".lua";
    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.path().extension().string() == extension) {
                if (!loadScript(entry.path().string())) return false;
            }
        }
    } catch (std::filesystem::filesystem_error& aaa) {
        logWarn("LuaManager: No existe ruta scripts.");
    }
    return true;
}

int LuaManager::print(lua_State* L) {
    std::stringstream aux;
    int i = 1;
    while (lua_gettop(L) >= i) {
        if (lua_isboolean(L, i)) {
            aux << (lua_toboolean(L, i) ? "true " : "false ");
        }
        else if (lua_iscfunction(L, i)) {
            aux << "C function ";
        }
        else if (lua_isfunction(L, i)) {
            aux << "Lua function ";
        }
        else if (lua_isnil(L, i)) {
            aux << "nil ";
        }
        else if (lua_isinteger(L, i)) {
            aux << lua_tointeger(L, i) << ' ';
        }
        else if (lua_isnumber(L, i)) {
            aux << lua_tonumber(L, i) << ' ';
        }
        else if (lua_isstring(L, i)) {
            aux << lua_tostring(L, i) << ' ';
        }
        else if (lua_istable(L, i)) {
            aux << "table ";
        }
        else if (lua_isthread(L, i)) {
            aux << "thread ";
        }
        else if (lua_isuserdata(L, i)) {
            aux << "userdata ";
        }
        else {
            aux << "something ";
        }
        i++;
    }
    std::cout << "[INFO|JUEGO] " << aux.str() << '\n';
    return 0;
}

}