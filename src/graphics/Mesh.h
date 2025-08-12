#pragma once
#include <glad/glad.h>
#include <vector>

template <GLuint VSize>
struct Mesh
{
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    static constexpr GLuint vertSize{VSize};
};
