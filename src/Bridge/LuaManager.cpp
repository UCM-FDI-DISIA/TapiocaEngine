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

namespace Tapioca {
LuaManager* LuaManager::instance_ = nullptr;

LuaManager::LuaManager() : L(nullptr), initialized(true) {
    L = luaL_newstate();
    luaL_openlibs(L);

    luabridge::getGlobalNamespace(L).addFunction("print", &print);
    if (!luaL_dofile(L, "test.lua")) {
        logError(("LuaManager: Error al cargar test.lua: " + std::string(lua_tostring(L, -1))).c_str());
        initialized = false;
    }

    reg = new LuaRegistry(L);
}

LuaManager::~LuaManager() {
    delete reg;
    lua_close(L);
}


bool LuaManager::init() {
    return initialized && loadBase() && loadScripts();
}

bool LuaManager::callLuaFunction(std::string name, const std::vector<CompValue>& parameters) {
    luabridge::LuaRef function = luabridge::getGlobal(L, "_internal")["call"];
    if (!function.isCallable()) return false;
    luabridge::LuaResult result = function(name, parameters);
    if (result.hasFailed()) {
        logError(("LuaManager: Error al ejecutar la funcion de Lua \"" + name + "\" [" +
                  std::to_string(result.errorCode().value()) + "]: " + result.errorMessage())
                     .c_str());
        return false;
    }
    return true;
}

bool LuaManager::loadBase() {
    if (luaL_dofile(L, BASE_FILE) != 0) {
        logError(("LuaManager: Error al cargar base de LuaComponent: " + std::string(lua_tostring(L, -1))).c_str());
        return false;
    }
    return true;
}

bool LuaManager::loadScript(const std::filesystem::path & path) {
    if (!loadBase()) return false;
    
    if (luaL_dofile(L, path.string().c_str()) != 0) {
        logError(("LuaManager: Error al cargar el archivo Lua: " + std::string(lua_tostring(L, -1))).c_str());
        return false;
    }
    luabridge::LuaRef* table = new luabridge::LuaRef(luabridge::getGlobal(L, "comp"));
    std::string name = path.filename().string();
    // Se le quita la extension (.lua)
    name = name.substr(0, name.length() - 4);
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