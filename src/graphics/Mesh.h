#pragma once
#include <glad/glad.h>
#include <vector>
#include <array>
#include <numeric>

template <GLuint VSize, GLuint... Layout>
struct Mesh
{
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    static constexpr GLuint vertSize{VSize};
    static constexpr std::array<GLuint, sizeof...(Layout)> layout{Layout...};
    static_assert(std::accumulate(layout.begin(), layout.end(), 0) == vertSize, "Incorrect layout");
};
