#version 430 core

in vec3 normal;

out vec4 fColor;

void main()
{
    fColor = vec4(1.0f, normal.y, 0.0f, 1.0f);
}
