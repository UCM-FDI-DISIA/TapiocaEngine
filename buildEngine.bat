@echo off

rem Directorios
set BAT_DIR=%~dp0
set SCRIPTS_DIR=%BAT_DIR%Dependencies\Scripts\
set SLN_PATH=%BAT_DIR%TapiocaEngine.sln

echo ~~Compilando dependencias...~~
echo.
if not exist %BAT_DIR%Dependencies\Ogre\build\x64\bin (
	call %SCRIPTS_DIR%buildOgre.bat
)
if not exist %BAT_DIR%Dependencies\Bullet\build\x64\x64 (
	call %SCRIPTS_DIR%buildBullet.bat
)
if not exist %BAT_DIR%Dependencies\Lua\build\x64\Debug (
	call %SCRIPTS_DIR%buildLua.bat
)

echo ~~Dependencias compiladas. Compilando motor...~~
echo.
msbuild %SLN_PATH% /p:configuration=Debug /t:Main /p:Platform=x64 /p:PlatformToolset=v143
msbuild %SLN_PATH% /p:configuration=Release /t:Main /p:Platform=x64 /p:PlatformToolset=v143

echo ~~Todo compilado. :YIPEEE:~~
pause