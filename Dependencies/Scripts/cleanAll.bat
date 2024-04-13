@echo off

set BAT_DIR=%~dp0

rem Limpia las dependencias
call %BAT_DIR%cleanOgre.bat
call %BAT_DIR%cleanBullet.bat
call %BAT_DIR%cleanLua.bat
rem cleans del resto de dependencias