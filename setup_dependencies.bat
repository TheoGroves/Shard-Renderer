
@echo off
setlocal

git clone https://github.com/KhronosGroup/Vulkan-Headers.git dependencies/Vulkan
if errorlevel 1 exit /b 1


git clone https://github.com/glfw/glfw.git dependencies/GLFW
if errorlevel 1 exit /b 1

cmake -S dependencies/Vulkan -B build/VulkanHeaders -DCMAKE_INSTALL_PREFIX="%CD%/dependencies/Vulkan/install"
if errorlevel 1 exit /b 1

cmake --build build/VulkanHeaders --config Release --target install
if errorlevel 1 exit /b 1

git clone https://github.com/KhronosGroup/Vulkan-Loader.git dependencies/VulkanLoader
if errorlevel 1 exit /b 1

cmake -S dependencies/VulkanLoader -B build/VulkanLoader -DVULKAN_HEADERS_INSTALL_DIR="%CD%/dependencies/Vulkan/install" -DCMAKE_INSTALL_PREFIX="%CD%/dependencies/VulkanLoader/install" -DCMAKE_INSTALL_LIBDIR=lib
if errorlevel 1 exit /b 1

cmake --build build/VulkanLoader --config Release
if errorlevel 1 exit /b 1

cmake --build build/VulkanLoader --config Release --target install
if errorlevel 1 exit /b 1

echo.
echo Dependencies setup successfuly.

endlocal
