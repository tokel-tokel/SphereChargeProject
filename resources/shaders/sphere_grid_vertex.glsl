#version 430 core

layout (location = 0) in vec2 position;

uniform float radius;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    float phi = position.x;
    float theta = position.y;
    vec4 modelPos = vec4(radius * sin(phi) * sin(theta), radius * cos(theta), radius * cos(phi) * sin(theta), 1.0f);
    gl_Position = projection * view * modelPos - vec4(0.0f, 0.0f, 0.001f, 0.0f);
}
