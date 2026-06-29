#version 460 core
layout(location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    mat4 mvp = uProj * uView * uModel;
    gl_Position = mvp * vec4(aPos, 1.0);
}