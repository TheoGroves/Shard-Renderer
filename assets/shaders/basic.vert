#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vPos;
out vec2 vUV;
out vec3 vNormal;
out vec3 vTangent;

void main()
{
    vUV = aUV;
    vNormal = mat3(uModel) * aNormal;
    vTangent = aTangent;

    vec4 worldPos = uModel * vec4(aPos, 1.0);
    vPos = worldPos.xyz;

    mat4 mvp = uProj * uView * uModel;
    gl_Position = mvp * vec4(aPos, 1.0);
}