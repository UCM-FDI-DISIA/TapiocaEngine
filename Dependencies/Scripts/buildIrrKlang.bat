set BAT_DIR=%~dp0
set BIN_DIR=%BAT_DIR%..\..\bin
XCOPY %BAT_DIR%..\IrrKlang\dlls\*.dll %BIN_DIR% /Y