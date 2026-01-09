@ECHO OFF
REM Build OWN CODE

ECHO "Building everything..."

PUSHD Engine
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
ECHO "Engine Build successfully."

PUSHD TestBench
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
ECHO "TestBench Build successfully."

ECHO "All assemblies built successfully."