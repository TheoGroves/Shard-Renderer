#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>

class Shader
{
public:
    Shader() = default;
    ~Shader();

    bool LoadFromFile(const std::string& vertexPath,
                      const std::string& fragmentPath);

    void Use() const;

private:
    GLuint mProgram = 0;

    std::string LoadFile(const std::string& path);
    GLuint Compile(GLenum type, const std::string& source);
};