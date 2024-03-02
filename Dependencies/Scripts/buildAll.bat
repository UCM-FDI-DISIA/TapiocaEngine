@echo off

set BAT_DIR=%~dp0

rem Limpia las dependencias
call %BAT_DIR%cleanOgre.bat
call %BAT_DIR%cleanBullet.bat
call %BAT_DIR%cleanLua.bat
rem cleans del resto de dependencias

rem Construye las dependencias
call %BAT_DIR%buildOgre.bat
call %BAT_DIR%buildBullet.bat
call %BAT_DIR%buildLua.bat
rem builds del resto de dependencias

pause
