#version 460 core
out vec4 FragColor;

uniform sampler2D uAlbedo;

in vec3 vPos;
in vec2 vUV;
in vec3 vNormal;
in vec3 vTangent;

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(vec3(-1.0, 1.0, -0.2));

    float diff = max(dot(N, L), 0.0);

    vec3 albedo = texture(uAlbedo, vUV).rgb;

    FragColor = vec4(albedo*diff, 1.0);
}