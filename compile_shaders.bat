@echo off

set "SHADER_DIR=shaders"

for /r "%SHADER_DIR%" %%f in (*.vert) do (
    glslc "%%f" -o "%%f.spv"
    if errorlevel 1 exit /b 1
)

for /r "%SHADER_DIR%" %%f in (*.frag) do (
    glslc "%%f" -o "%%f.spv"
    if errorlevel 1 exit /b 1
)

echo Shaders compiled.
exit /b 0