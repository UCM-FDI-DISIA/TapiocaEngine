
@echo off

rem Directorios
set "VS_INSTALL_PATH=%~1"
set "MS_BUILD_PATH=%~2"
set CMAKE_PATH=%VS_INSTALL_PATH%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin

rem Cambia al directorio de scripts
cd ..\..\Dependencies\Scripts

rem Comprueba que exista el directorio de cmake
if not exist %CMAKE_PATH% goto :error_setting_path

rem Limpia las dependencias
call cleanOgre.bat
call cleanBullet.bat
rem cleans del resto de dependencias

rem Construye las dependencias
call buildOgre.bat "%CMAKE_PATH%" "%MS_BUILD_PATH%"
call buildBullet.bat "%CMAKE_PATH%"" "%MS_BUILD_PATH%"
rem builds del resto de dependencias

pause

:error_setting_path
exit /B 1