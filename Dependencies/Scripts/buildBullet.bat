@echo off

rem Directorios
set BAT_DIR=%~dp0
set BULLET_SRC= %BAT_DIR%..\Bullet\src
set BULLET_BUILD= %BAT_DIR%..\Bullet\build\x64
set BULLET_SLN= %BAT_DIR%..\Bullet\build\x64\BULLET_PHYSICS.sln

rem Se crea el directorio si no está ya creado
if not exist %BULLET_BUILD% (
	mkdir %BULLET_BUILD%

	echo Comenzando la ejecucion de cmake
	cmake ^
		-D CMAKE_CONFIGURATION_TYPES:STRING=Debug;Release ^
		-D BUILD_BULLET_ROBOTICS_EXTRA:BOOL=0 ^
		-D BUILD_BULLET_ROBOTICS_GUI_EXTRA:BOOL=0 ^
		-D BUILD_CLSOCKET:BOOL=0 ^
		-D BUILD_ENET:BOOL=0 ^
		-D BUILD_UNIT_TESTS:BOOL=0 ^
		-D ENABLE_VHACD:BOOL=0 ^
		-D BUILD_BULLET2_DEMOS:BOOL=0 ^
		-D BUILD_CPU_DEMOS:BOOL=0 ^
		-D BUILD_OPENGL3_DEMOS:BOOL=0 ^
		-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=1 ^
	-S %BULLET_SRC% -B %BULLET_BUILD%
)

if exist %BULLET_SLN% (
	rem Compilacion de la solucion en Debug y en Release
	msbuild %BULLET_SLN% /p:configuration=Debug /t:ALL_BUILD /p:Platform=x64 /p:PlatformToolset=v143
	msbuild %BULLET_SLN% /p:configuration=Release /t:ALL_BUILD /p:Platform=x64 /p:PlatformToolset=v143
	echo Terminada la build de Bullet
) else echo No se ha encontrado el archivo %BULLET_SLN%
