@echo off
setlocal

echo Configuring renderer...

cmake -S . -B build/Renderer
if errorlevel 1 exit /b 1

echo Renderer config successful.

echo Building renderer...

cmake --build build/Renderer --config Release
if errorlevel 1 exit /b 1

echo.
echo Build successful.

endlocal
exit /b 0