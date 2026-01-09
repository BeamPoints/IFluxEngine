@echo off
:: Lädt die MSVC-Tools (msbuild, cl, link)
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul

echo Building cglm...
msbuild cglm.vcxproj /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v143

if %ERRORLEVEL% neq 0 (
    echo Build failed with Error: %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)