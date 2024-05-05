#pragma once
#include "defs.h"
#include <lua.hpp>
#include <LuaBridge.h>

struct lua_State;

namespace Tapioca {
/**
* @brief Registra clases, funciones y valores en Lua y permite acceder a los valores registrados
*/
class TAPIOCA_API LuaRegistry {
private:
    lua_State* L;            // Estado do Lua

    // warning C4251 'Tapioca::LuaRegistry::map' :
    // class 'luabridge::LuaRef' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::LuaRegistry'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    luabridge::LuaRef map;   // Mapa de valores
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

public:
    /**
    * @brief Crea una tabla de valores en Lua
    * @param L Estado de Lua
    */
    LuaRegistry(lua_State* L);

    /**
    * @brief Registra una funcion en Lua
    * @param className Nombre de la clase
    * @param functionName Nombre de la funcion
    * @param function Funcion a registrar
    */
    template<typename TFunc>
    void registerFunction(std::string className, std::string functionName, TFunc function) {
        luabridge::getGlobalNamespace(L)
            .beginNamespace("Tapioca")
            .beginNamespace(className.c_str())
            .addFunction(functionName, function)
            .endNamespace()
            .endNamespace();
    }

    /**
    * @brief Devuelve un valor de Lua
    * @param name Nombre del valor
    * @return Valor
    */
    template<typename T>
    T getValueFromLua(std::string name) {
        return map[name].cast<T>();
    }
};
}
