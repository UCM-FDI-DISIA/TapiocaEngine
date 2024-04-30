@echo off

rem Directorios
set BAT_DIR=%~dp0
set SCRIPTS_DIR=%BAT_DIR%\Dependencies\Scripts\
set SLN_PATH=%BAT_DIR%\TapiocaEngine.sln
set BIN_DIR=%BAT_DIR%\bin

:parse
if "%~1"=="" goto endparse
if "%~1"=="--nodeb" set /a nodeb=1
if "%~1"=="--norel" set /a norel=1
if "%~1"=="--nopause" set /a nopause=1
shift
goto parse
:endparse

echo ~~Compilando dependencias...~~
echo.
if not exist %BAT_DIR%\Dependencies\Ogre\build\x64\bin (
	call %SCRIPTS_DIR%\buildOgre.bat
)
if not exist %BAT_DIR%\Dependencies\Bullet\build\x64\x64 (
	call %SCRIPTS_DIR%\buildBullet.bat
)
if not exist %BAT_DIR%\Dependencies\Lua\build\x64\Debug (
	call %SCRIPTS_DIR%\buildLua.bat
)

call %SCRIPTS_DIR%\buildIrrKlang.bat

echo ~~Dependencias compiladas. Compilando motor...~~
echo.
if not defined nodeb msbuild %SLN_PATH% /p:configuration=Debug /p:Platform=x64 /p:PlatformToolset=v143
if not defined norel msbuild %SLN_PATH% /p:configuration=Release /p:Platform=x64 /p:PlatformToolset=v143

echo ~~Todo compilado. :YIPEEE:~~
if not defined nopause pause
