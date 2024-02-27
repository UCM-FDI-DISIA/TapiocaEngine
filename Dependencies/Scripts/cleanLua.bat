@echo off

rem Variable de directorio
set LUA_BUILD_DIR=..\Lua\build\

echo Borrando build de Lua
if exist %LUA_BUILD_DIR% (
    rmdir /s /q %LUA_BUILD_DIR%
    echo Borrada build de Lua.
) else (
    echo No se ha encontrado directorio build en Lua\build. No se realiza nada.
)
