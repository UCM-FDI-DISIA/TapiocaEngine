@echo off

rem Directorios
set BAT_DIR=%~dp0
set LUA_SRC=%BAT_DIR%..\Lua
set LUA_BUILD=%BAT_DIR%..\Lua\build\x64
set LUA_SLN=%BAT_DIR%..\Lua\build\x64\LUA_C.sln

rem Se crea el directorio si no está ya creado
if not exist %LUA_BUILD% (
    mkdir %LUA_BUILD%

    echo Comenzando la ejecucion de cmake
    cmake ^
        -D CMAKE_CONFIGURATION_TYPES:STRING=Debug;Release ^
        -S %LUA_SRC% -B %LUA_BUILD%
)
if exist %LUA_SLN% (
    rem Compilacion de la solucion en Debug y en Release
    msbuild %LUA_SLN% /p:configuration=Debug /t:lua /p:Platform=x64 /p:PlatformToolset=v143
    msbuild %LUA_SLN% /p:configuration=Release /t:lua /p:Platform=x64 /p:PlatformToolset=v143
    echo Terminada la build de Lua
) else echo No se ha encontrado el archivo %LUA_SLN%

