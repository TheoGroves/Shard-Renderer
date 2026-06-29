call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
cl /EHsc /MD /std:c++20 /D GLFW_STATIC /I dependencies/include ^
src/main.cpp src/Platform/Windows/linking.cpp src/window.cpp src/engine.cpp src/shader.cpp src/Platform/Windows/gpuhints.cpp ^
/link /LIBPATH:dependencies/lib /NODEFAULTLIB:LIBCMT
timeout /t 5