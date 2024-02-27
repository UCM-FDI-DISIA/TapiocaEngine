

#include <lua.hpp>
#include <LuaBridge.h>

#include <fstream>
#include <iostream>
#include "lua1.h"

int main(int argc, char** argv) {


   lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    if (luaL_dofile(L, "archivo.json") != 0) {
        // Manejo de errores
        std::cerr << "Error al cargar el archivo JSON: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return -1;
    }
    return 0;
}

//int Tapioca::lua1::Prueba() {
//
//    lua_State* L = luaL_newstate();
//    luaL_openlibs(L);
//    if (luaL_dofile(L, "archivo.json") != 0) {
//        // Manejo de errores
//        std::cerr << "Error al cargar el archivo JSON: " << lua_tostring(L, -1) << std::endl;
//        lua_close(L);
//        return -1;
//    }
//    return 0;
//}
