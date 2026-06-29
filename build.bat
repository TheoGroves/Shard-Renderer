call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
cl /EHsc /MD /std:c++20 /D GLFW_STATIC /I dependencies/include /I src ^
src/main.cpp ^
src/Platform/Windows/linking.cpp ^
src/Rendering/window.cpp ^
src/Rendering/shader.cpp ^
src/Core/engine.cpp ^
src/Platform/Windows/gpuhints.cpp ^
/link /LIBPATH:dependencies/lib /NODEFAULTLIB:LIBCMT
timeout /t 5