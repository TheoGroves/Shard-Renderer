#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include "Math/Mat4.h"

class Shader
{
public:
    Shader() = default;
    ~Shader();

    bool LoadFromFile(const std::string& vertexPath,
                      const std::string& fragmentPath);

    void Use() const;

    void SetMat4(const std::string& name, const Mat4& mat);

private:
    GLuint mProgram = 0;

    std::string LoadFile(const std::string& path);
    GLuint Compile(GLenum type, const std::string& source);
};