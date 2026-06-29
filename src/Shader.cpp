#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string Shader::LoadFile(const std::string& path)
{
    std::ifstream file(path);
    std::stringstream ss;

    ss << file.rdbuf();
    return ss.str();
}

GLuint Shader::Compile(GLenum type, const std::string& source)
{
    GLuint shader = glCreateShader(type);

    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char log[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, log);
        std::cout << "[SHADER ERROR] " << log << "\n";
    }

    return shader;
}

bool Shader::LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vs = LoadFile(vertexPath);
    std::string fs = LoadFile(fragmentPath);

    GLuint vert = Compile(GL_VERTEX_SHADER, vs);
    GLuint frag = Compile(GL_FRAGMENT_SHADER, fs);

    mProgram = glCreateProgram();
    glAttachShader(mProgram, vert);
    glAttachShader(mProgram, frag);
    glLinkProgram(mProgram);

    GLint success;
    glGetProgramiv(mProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        char log[1024];
        glGetProgramInfoLog(mProgram, 1024, nullptr, log);
        std::cout << "[PROGRAM ERROR] " << log << "\n";
        return false;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    return true;
}

void Shader::Use() const
{
    glUseProgram(mProgram);
}

Shader::~Shader()
{
    if (mProgram)
        glDeleteProgram(mProgram);
}