#version 450

layout(location = 0) in vec2 position; // First vertex attribute
layout(location = 1) in vec3 color; // Second vertex attribute

layout(location = 0) out vec3 fragColor; // There is no association between input location and output location 

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    fragColor = color;
}