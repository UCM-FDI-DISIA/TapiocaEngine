@echo off

rem Este .bat se encuentra en el directorio Dependencies/Scripts

rem Variable de directorio
set OGRE_BUILD_DIR=..\Ogre\build\

echo Borrando build de Ogre
if exist %OGRE_BUILD_DIR% (
    rmdir /s /q %OGRE_BUILD_DIR%
    echo Borrada build de Ogre.
) else (
    echo No se ha encontrado directorio build en Ogre\build\. No se realiza nada.
)

pause
