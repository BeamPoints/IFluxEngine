:: BUILD SCRIPTS FÜR HIGHWAY Windows <Ninja wird benötigt>
:: build_Highway.bat wichtig im root vom Highway Folder abspeichern

@echo off
setlocal enabledelayedexpansion

:: Pfade definieren
set "BASE_DIR=%~dp0"
set "TEMP_DIR=%BASE_DIR%bin\temp"
set "BIN_DIR=%BASE_DIR%bin"

echo ======================================================
echo Highway Rebuild: Clang 19 + Ninja
echo ======================================================

:: 1. Alten Temp-Ordner loeschen (der radikale Reset)
if exist "%TEMP_DIR%" (
    echo [1/3] Clean: Entferne alten Temp-Ordner...
    rmdir /s /q "%TEMP_DIR%"
)

:: 2. CMake Konfiguration
echo [2/3] Configure: Erstelle Build-Dateien...
cmake -B "%TEMP_DIR%" -G "Ninja" ^
  -DCMAKE_C_COMPILER="clang-cl.exe" ^
  -DCMAKE_CXX_COMPILER="clang-cl.exe" ^
  -DHWY_ENABLE_INSTALL=ON ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_INSTALL_PREFIX="%BIN_DIR%"

if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake-Konfiguration fehlgeschlagen!
    pause
    exit /b %ERRORLEVEL%
)

:: 3. Bauen und Installieren
echo [3/3] Build: Kompiliere und installiere Highway...
cmake --build "%TEMP_DIR%" --target install

if %ERRORLEVEL% neq 0 (
    echo ERROR: Build fehlgeschlagen!
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo ======================================================
echo REBUILD ERFOLGREICH!
echo Libs und Header liegen in: %BIN_DIR%
echo ======================================================
pause