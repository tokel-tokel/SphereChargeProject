#pragma once

#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include "Shader.h"
#include "objects/ChargedSphere.h"

struct SphereMesh
{
    std::vector<float> vertices; // layout 0 vec2(longitude in [0, 2pi), latitude in [0, pi]) layout 1 normal vec3
    std::vector<GLuint> indices; // CCW face
    static constexpr size_t vertSize{5};
    static constexpr float deltaThetaMultip{0.4f};
    static constexpr float pi{glm::pi<float>()};
    static SphereMesh createSphere(size_t meridians, size_t parallels);
private:
    static constexpr float calcTheta(size_t k, size_t count)
    {
        return pi / 2 + glm::sqrt(deltaThetaMultip) * glm::tan(glm::atan(pi / (2 * glm::sqrt(deltaThetaMultip))) *
            (static_cast<float>(2 * k) / (count - 1) - 1.0f));
    }
};

class SphereRenderer 
{
private:
    ChargedSphere& sphere;
    Shader shader;
    SphereMesh mesh;
public:
    SphereRenderer(ChargedSphere& sphere, Shader&& shader, SphereMesh&& mesh);
};
