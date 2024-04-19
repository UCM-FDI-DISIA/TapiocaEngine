set BAT_DIR=%~dp0
set BIN_DIR=%BAT_DIR%..\..\bin
XCOPY %BAT_DIR%..\IrrKlang\dlls\irrKlang.dll %BIN_DIR% /Y
XCOPY %BAT_DIR%..\IrrKlang\dlls\ikpMP3.dll %BIN_DIR% /Y
XCOPY %BAT_DIR%..\IrrKlang\dlls\ikpFlac.dll %BIN_DIR% /Y