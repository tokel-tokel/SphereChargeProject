#version 430 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 inNormal;

uniform float solidConeCos;
uniform vec2 solidPos;
uniform float radius;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 pointColor;

float haverSin(float angle)
{
    float halfSin = sin(angle / 2);
    return halfSin * halfSin;
}

void main()
{
    float phi = position.x;
    float theta = position.y;
    vec3 normalPos = vec3(sin(phi) * sin(theta), cos(theta), cos(phi) * sin(theta));
    gl_Position = projection * view * vec4(radius * normalPos, 1.0f);

    vec3 solidNormalPos = vec3(sin(solidPos.x) * sin(solidPos.y), cos(solidPos.y), cos(solidPos.x) * sin(solidPos.y));

    float delta = dot(normalPos, solidNormalPos) - solidConeCos;
    float smoothFactor = smoothstep(-0.1, 0.1, delta);
    pointColor = mix(vec3(0.810f, 0.713f, 0.774f), vec3(0.173f, 1.0f, 0.345f), smoothFactor);
    //if (delta > 0) pointColor = vec3(0.173f, 1.0f, 0.345f);
    //else pointColor = vec3(0.810f, 0.713f, 0.774f);
    normal = inNormal;

}
