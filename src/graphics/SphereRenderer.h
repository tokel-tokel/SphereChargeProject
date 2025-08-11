#pragma once

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
    static SphereMesh createSphere(GLuint meridians, GLuint parallels);
private:
    static constexpr float pi{glm::pi<float>()};
    static constexpr float deltaThetaMultip{0.4f};
    static constexpr float calcTheta(GLuint k, GLuint count)
    {
        return pi / 2 + glm::sqrt(deltaThetaMultip) * glm::tan(glm::atan(pi / (2 * glm::sqrt(deltaThetaMultip))) *
            (static_cast<float>(2 * k) / (count - 1) - 1.0f));
    }
};

class SphereRenderer 
{
private:
    OpenGLContext context;
    std::reference_wrapper<ChargedSphere> sphere;
    Shader shader;
    SphereMesh mesh;
    GLuint VAO, VBO, EBO;
public:
    SphereRenderer(OpenGLContext context, ChargedSphere& sphere, Shader&& shader, SphereMesh&& mesh);
    SphereRenderer(const SphereRenderer&) = delete;
    SphereRenderer& operator=(const SphereRenderer&) = delete;
    SphereRenderer(SphereRenderer&&) = default;
    SphereRenderer& operator=(SphereRenderer&&) = default;

    void render(float radius) const;
    ~SphereRenderer();
};

class SphereRendererBuilder
{
private:
    SphereMesh mesh;
    std::optional<OpenGLContext> context;
    std::optional<Shader> shader;
    std::optional<std::reference_wrapper<ChargedSphere>> sphere;
public:
    SphereRendererBuilder& setMesh(GLuint meridians, GLuint parallels);
    SphereRendererBuilder& setContext(OpenGLContext context_);
    SphereRendererBuilder& setShader(Shader&& shader_);
    SphereRendererBuilder& setReference(ChargedSphere& sphere);
    SphereRenderer build();
};
