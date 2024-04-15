#include "LuaManager.h"
#include <lua.hpp>
#include "LuaRegistry.h"
#include <LuaBridge.h>
#include <filesystem>
#include "Components/LuaComponent.h"
#include "Structure/FactoryManager.h"

namespace Tapioca {
LuaManager* LuaManager::instance_ = nullptr;

LuaManager::LuaManager() : L(nullptr) {
    L = luaL_newstate();
    luaL_openlibs(L);

    // TODO: Revisar
    luabridge::getGlobalNamespace(L).addFunction("print", [](const char* info) -> void {
        if (info == nullptr) logInfo("nil");
        else logInfo(info);
    });
    luaL_dofile(L, "assets/test.lua");

    reg = new LuaRegistry(L);
}

LuaManager::~LuaManager() {
    delete reg;
    lua_close(L);
}


bool LuaManager::init() {
    loadScripts();
    return true;
}

void LuaManager::loadBase() {
    // TODO Cambiar nombre (xD)
    const char* mequieromorir = R"(comp = {
    new = function(self)
        local newObj = {}
        setmetatable(newObj, { __index = self })
        print("Me he creado :YIPEE:")
        return newObj
    end,
    start = function(self) end,
    awake = function(self) end,
    initComponent = function(self) end,
    update = function(self) end,
    fixedUpdate = function(self) end,
    render = function(self) end,
    handleEvent = function(self) end,
    -- Con implementacion
    pushEvent = function(self) end,
    object = nil,
    alive = true,
    active = true
    })";
    if (luaL_dostring(L, mequieromorir) != 0) {
        logError(("LuaManager: Error al cargar base de LuaComponent: " + std::string(lua_tostring(L, -1))).c_str());
    }
}

bool LuaManager::loadScript(const std::filesystem::path & path) {
    loadBase();
    
    if (luaL_dofile(L, path.string().c_str()) != 0) {
        logError(("LuaManager: Error al cargar el archivo Lua: " + std::string(lua_tostring(L, -1))).c_str());
        return false;
    }
    luabridge::LuaRef* table = new luabridge::LuaRef(luabridge::getGlobal(L, "comp"));
    std::string name = path.filename().string();
    name = name.substr(0, name.length() - 4);
    // TODO: Mirar si se puede quitar esto
    luabridge::getGlobalNamespace(L)
        .beginNamespace("Components")
        .addVariable(name.c_str(), table)
        .endNamespace();
    FactoryManager::instance()->addBuilder(new LuaComponentBuilder(name, table)); // Desde 18:40 a 21:15, Desde 22:22 a 00:45
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
        // TODO Cambiar a excepci�n correcta (std::filesystem_error?????)
    } catch (...) {
        logWarn("LuaManager: No existe ruta scripts.");
    }
    return true;
}

}