#pragma once

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "objects/ChargedSphere.h"

struct SphereMesh : Mesh<5>
{
    static SphereMesh createSphere(GLuint meridians, GLuint parallels);
    static constexpr std::array<GLuint, 2> inParamSize{2, 3};
private:
    static constexpr float pi{glm::pi<float>()};
    static constexpr float deltaThetaMultip{0.7f};
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
    SphereRenderer(OpenGLContext context, ChargedSphere& sphere_, Shader&& shader_, SphereMesh&& mesh_);
    SphereRenderer(const SphereRenderer&) = delete;
    SphereRenderer& operator=(const SphereRenderer&) = delete;
    SphereRenderer(SphereRenderer&& other) noexcept;
    SphereRenderer& operator=(SphereRenderer&& other) noexcept;
    ~SphereRenderer();

    void render(float radius) const;
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
