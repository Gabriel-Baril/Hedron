#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 uv;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform GlobalUbo
{
    mat4 projectionViewMatrix;
    vec3 directionToLight;
} ubo;

layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

const float AMBIENT = 0.02;

void main()
{
    gl_Position = ubo.projectionViewMatrix * push.modelMatrix * vec4(position, 1.0); // Since "position" is a position, the homogenous coordinate needs to be 1

    // only works if scale is uniform (sx == sy == sz)
    // vec3 normalWorldSpace = normalize(mat3(push.modelMatrix) * normal);

    // Calculating the inverse in a shader can be expensive and should be avoided, prefer uniform scaling if possible
    // mat3 normalMatrix = transpose(inverse(mat3(push.modelMatrix)));
    // vec3 normalWorldSpace = normalize(normalMatrix * normal);

    vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

    float lightIntensity = AMBIENT + max(dot(normalWorldSpace, ubo.directionToLight), 0);
    fragColor = lightIntensity * color;
}