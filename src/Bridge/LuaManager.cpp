#include "LuaManager.h"
#include <lua.hpp>
#include "LuaRegistry.h"
#include <LuaBridge.h>
#include <filesystem>

namespace Tapioca {
LuaManager::LuaManager() : L(nullptr) {
    L = luaL_newstate();
    reg = new LuaRegistry();
}

LuaManager::~LuaManager() {
    delete reg;
    lua_close(L);
}


bool LuaManager::init() { return true; }

void LuaManager::loadBase() { luaL_dostring(L, 
    "comp = {\
    new = function(self)\
        local newObj = {}\
        newObj.__index = self\
        return newObj\
    end,\
    start = function(self) end,\
    awake = function(self) end,\
    initComponent = function(self) end,\
    update = function(self) end,\
    fixedUpdate = function(self) end,\
    render = function(self) end,\
    handleEvent = function(self) end,\
    -- Con implementacion\
    pushEvent = function(self) end,\
    object = nil,\
    alive = true,\
    active = true\
    }"
); }

bool LuaManager::loadScript(const std::filesystem::path & path) {
    loadBase();
    
    if (luaL_dofile(L, path.string().c_str()) != 0) {
        logError(("LuaComponent: Error al cargar el archivo Lua: " + std::string(lua_tostring(L, -1))).c_str());
        return false;
    }
    luabridge::LuaRef* table = new luabridge::LuaRef(luabridge::getGlobal(L, "comp"));
    std::string name = path.filename().string();
    luabridge::getGlobalNamespace(L)
        .beginNamespace("Components")
        .addVariable(name.substr(0, name.length() - 4).c_str(), table)
        .endNamespace();
    return false;
}

void LuaManager::loadScripts() {
    std::string path = "assets\\scripts\\";   // Reemplaza esto con tu ruta
    std::string extension = ".lua";
    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.path().extension().string() == extension) {
                loadScript(entry.path().string());
            }
        }
    } catch (...) {
        logWarn("LuaManager: No existe ruta scripts.");
    }
}

}