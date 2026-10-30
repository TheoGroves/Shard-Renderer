
@echo off
setlocal

call :CloneOrUpdate https://github.com/KhronosGroup/Vulkan-Headers.git dependencies/Vulkan
call :CloneOrUpdate https://github.com/glfw/glfw.git dependencies/GLFW
call :CloneOrUpdate https://github.com/KhronosGroup/Vulkan-Loader.git dependencies/VulkanLoader
call :CloneOrUpdate https://github.com/google/googletest.git dependencies/GoogleTest

cmake -S dependencies/Vulkan -B build/VulkanHeaders -DCMAKE_INSTALL_PREFIX="%CD%/dependencies/Vulkan/install"
if errorlevel 1 exit /b 1

cmake --build build/VulkanHeaders --config Release --target install
if errorlevel 1 exit /b 1

cmake -S dependencies/VulkanLoader -B build/VulkanLoader -DVULKAN_HEADERS_INSTALL_DIR="%CD%/dependencies/Vulkan/install" -DCMAKE_INSTALL_PREFIX="%CD%/dependencies/VulkanLoader/install" -DCMAKE_INSTALL_LIBDIR=lib
if errorlevel 1 exit /b 1

cmake --build build/VulkanLoader --config Release
if errorlevel 1 exit /b 1

cmake --build build/VulkanLoader --config Release --target install
if errorlevel 1 exit /b 1

cmake -S dependencies/GoogleTest -B build/GoogleTest -DCMAKE_INSTALL_PREFIX="%CD%/dependencies/GoogleTest/install"
if errorlevel 1 exit /b 1

cmake --build build/GoogleTest --config Release
if errorlevel 1 exit /b 1

cmake --build build/GoogleTest --config Release --target install
if errorlevel 1 exit /b 1

echo.
echo Dependencies setup successfully.

endlocal
exit /b 0

:CloneOrUpdate
if exist "%~2\.git" (
    git -C "%~2" pull
    exit /b %errorlevel%
) else (
    git clone %~1 "%~2"
    exit /b %errorlevel%
)