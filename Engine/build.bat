REM Build script for engine
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Build Windows Resource File
ECHO "Building Resources..."
SET RC_PATH="C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x64\rc.exe"
pushd src\Platforms\Utility\
:: Jetzt den Befehl mit dem vollen Pfad aufrufen
%RC_PATH% /nologo /fo asset.res asset.rc
popd
ECHO "Copying Resources to Build Directory..."
copy src\Platforms\Utility\asset.res ..\TestBench\asset.res

REM Get a list of all the .c files.
SET cFilenames=
FOR /R %%f in (*.c) do (
    SET cFilenames=!cFilenames! %%f
)

 echo "Files:" %cFilenames%

SET assembly=engine
SET compilerFlags=-g -shared -Wvarargs -Wall -Werror
REM -Wall -Werror
SET includeFlags=-Isrc -I%VULKAN_SDK%/Include -I../ThirdParty/cglm-0.9.6/Include -I../ThirdParty/highway-1.3.0/bin/include
SET linkerFlags=-luser32 -lvulkan-1 -L%VULKAN_SDK%/Lib -L../ThirdParty/cglm-0.9.6/win/x64/Debug -L../ThirdParty/highway-1.3.0/bin/lib 
SET defines=-D_DEBUG -DFEXPORT -D_CRT_SECURE_NO_WARNINGS

ECHO "Building %assembly%%..."
clang %cFilenames% %compilerFlags% -o ../bin/%assembly%.dll %defines% %includeFlags% %linkerFlags%