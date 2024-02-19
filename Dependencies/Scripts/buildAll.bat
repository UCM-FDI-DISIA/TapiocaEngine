@echo off

rem Limpia las dependencias
call cleanOgre.bat
call cleanBullet.bat
rem cleans del resto de dependencias

rem Construye las dependencias
call buildOgre.bat
call buildBullet.bat
rem builds del resto de dependencias

pause