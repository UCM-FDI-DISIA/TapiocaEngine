@echo off

rem Directorios
set BAT_DIR=%~dp0
set OGRE_SRC=%BAT_DIR%..\Ogre\src
set OGRE_BUILD=%BAT_DIR%..\Ogre\build\x64
set OGRE_SLN=%BAT_DIR%..\Ogre\build\x64\OGRE.sln
set CMAKE_PATH=%1
set MS_BUILD_PATH=%2

rem Se crea el directorio si no está ya creado
if not exist %OGRE_BUILD% (
    mkdir %OGRE_BUILD%

    echo Comenzando la ejecucion de cmake
    %CMAKE_PATH%\cmake.exe ^
        -D CMAKE_CONFIGURATION_TYPES:STRING=Debug;Release ^
        -D OGRE_BUILD_COMPONENT_BULLET:BOOL=FALSE ^
        -D OGRE_BUILD_COMPONENT_BITES:BOOL=FALSE ^
        -D OGRE_BUILD_COMPONENT_TERRAIN:BOOL=FALSE ^
        -D OGRE_BUILD_COMPONENT_VOLUME:BOOL=FALSE ^
        -D OGRE_BUILD_PLUGIN_BSP:BOOL=FALSE ^
        -D OGRE_BUILD_PLUGIN_DOT_SCENE:BOOL=FALSE ^
        -D OGRE_BUILD_PLUGIN_DOT_SCENE:BOOL=FALSE ^
        -D OGRE_BUILD_PLUGIN_OCTREE:BOOL=FALSE ^
        -D OGRE_BUILD_PLUGIN_PCZ:BOOL=FALSE ^
        -D OGRE_BUILD_PLUGIN_STBI:BOOL=FALSE ^
        -D OGRE_BUILD_RENDERSYSTEM_D3D11:BOOL=FALSE ^
        -D OGRE_BUILD_RENDERSYSTEM_GL:BOOL=FALSE ^
        -D OGRE_BUILD_RENDERSYSTEM_GLES2:BOOL=FALSE ^
        -D OGRE_BUILD_SAMPLES:BOOL=FALSE ^
        -D OGRE_BUILD_TOOLS:BOOL=FALSE ^
        -D OGRE_INSTALL_DEPENDENCIES:BOOL=TRUE ^
        -D OGRE_INSTALL_DOCS:BOOL=FALSE ^
        -D OGRE_INSTALL_SAMPLES:BOOL=FALSE ^
        -D OGRE_INSTALL_TOOLS:BOOL=FALSE ^
        -D OGRE_INSTALL_VSPROPS:BOOL=TRUE ^
        -D OGRE_BUILD_PLUGIN_ASSIMP:BOOL=FALSE ^
    -S %OGRE_SRC% -B %OGRE_BUILD%
)
if exist %OGRE_SLN% (
    rem Compilacion de la solucion en Debug y en Release
    %MS_BUILD_PATH%\msbuild.exe %OGRE_SLN% /p:configuration=Debug /t:ALL_BUILD /p:Platform=x64 /p:PlatformToolset=v143
    %MS_BUILD_PATH%\msbuild.exe %OGRE_SLN% /p:configuration=Release /t:ALL_BUILD /p:Platform=x64 /p:PlatformToolset=v143
    cd ..
    XCOPY Ogre\build\x64\bin\release\*.dll ..\bin /Y
    XCOPY Ogre\build\x64\bin\debug\*.dll ..\bin /Y
    cd Scripts
    echo Terminada la build de Ogre
) else echo No se ha encontrado el archivo %OGRE_SLN%

pause
