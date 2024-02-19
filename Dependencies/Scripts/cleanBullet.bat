@echo off

rem Este .bat se encuentra en el directorio Dependencies/Scripts

rem Variable de directorio
set BULLET_BUILD_DIR=..\Bullet\build\

echo Borrando build de Bullet
if exist %BULLET_BUILD_DIR% (
    rmdir /s /q %BULLET_BUILD_DIR%
    echo Borrada build de Bullet.
) else (
    echo No se ha encontrado directorio build en Bullet\build. No se realiza nada.
)

pause