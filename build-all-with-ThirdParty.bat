@ECHO OFF
REM Build Everything

ECHO "Building everything..."

PUSHD ThirdParty

PUSHD "cglm-0.9.6\win"
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
ECHO "Dependencie cglm Build successfully."

PUSHD "highway-1.3.0"
CALL build_Highway.bat
POPD
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
ECHO "Dependencie highway Build successfully."

REM You can add other ThirdParty builds here if needed
POPD

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