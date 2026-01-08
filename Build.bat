@echo off
REM This batch file sets up the environment for building a project using Visual Studio 2022 Community Edition.
REM It configures the necessary environment variables for 64-bit development.

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

SET includes=/Isrc /I%VULKAN_SDK%/Include
SET links=/link /LIBPATH:%VULKAN_SDK%/Lib vulkan-1.lib
SET defines=

echo "Building the project..."

cl /EHsc %includes% %defines% src/main.cpp %links%