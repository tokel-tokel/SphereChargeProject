#version 430 core

layout (location = 0) in vec2 position;

uniform float aspect;

void main()
{
    vec2 pos = position;
    if(aspect < 1) pos.y *= aspect;
    else pos.x *= (1/aspect);
    gl_Position = vec4(pos, 1.0f, 1.0f);
}

