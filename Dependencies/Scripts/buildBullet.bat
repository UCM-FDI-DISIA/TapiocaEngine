@echo off

rem Este .bat se encuentra en el directorio Dependencies/Scripts

rem Directorios
set CMAKE_EXE=..\cmake\bin\cmake.exe
set BULLET_SRC=..\Bullet\src
set BULLET_BUILD=..\Bullet\build\x64
set BULLET_SLN=..\Bullet\build\x64\BULLET_PHYSICS.sln

rem Se crea el directorio si no está ya creado
if not exist %BULLET_BUILD% mkdir %BULLET_BUILD%
if exist %BULLET_SLN% goto end

echo Comenzando la ejecución de cmake

%CMAKE_EXE% -D CMAKE_CONFIGURATION_TYPES:STRING=Debug;Release ^
	-D BUILD_BULLET_ROBOTICS_EXTRA:BOOL=0 ^
	-D BUILD_BULLET_ROBOTICS_GUI_EXTRA:BOOL=0 ^
	-D BUILD_CLSOCKET:BOOL=0 ^
	-D BUILD_ENET:BOOL=0 ^
	-D ENABLE_VHACD:BOOL=0 ^
        -D BUILD_BULLET2_DEMOS:BOOL=0 ^
	-D BUILD_CPU_DEMOS:BOOL=0 ^
	-D BUILD_OPENGL3_DEMOS:BOOL=0 ^
	-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=1 ^
	-S %BULLET_SRC% -B %BULLET_BUILD%

rem Compilacion de la solucion en Debug y en Release
msbuild %BULLET_SLN% /p:configuration=Debug /t:ALL_BUILD /p:Platform=x64 /p:PlatformToolset=v143
msbuild %BULLET_SLN% /p:configuration=Release /t:ALL_BUILD /p:Platform=x64 /p:PlatformToolset=v143

echo Terminada la build de Bullet

:end