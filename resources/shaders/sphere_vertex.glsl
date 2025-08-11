#version 430 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 inNormal;

uniform float radius;

out vec3 normal;

void main()
{
    float phi = position.x;
    float theta = position.y;
    gl_Position = vec4(radius * sin(phi) * sin(theta), radius * cos(theta), radius * cos(phi) * sin(theta), 1.0f);
}
