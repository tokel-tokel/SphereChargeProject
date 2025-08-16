#version 430 core

in vec3 normal;
in vec3 pointColor;

out vec4 fColor;

void main()
{
    fColor = vec4(pointColor.r, pointColor.g + normal.y * 0.001, pointColor.b, 1.0f);
}
