#pragma once
#include "Mesh.h"
#include "OpenGLContext.h"
#include "Shader.h"
#include "glm/ext/scalar_constants.hpp"

struct SphereGridMesh : Mesh<2>
{
    static SphereGridMesh createGrid(GLuint meridians, GLuint parallels);
private:
    static constexpr float pi{glm::pi<float>()};
};

class SphereGridRenderer
{
private:
    OpenGLContext context;
    Shader shader;
    SphereGridMesh mesh;
    GLuint VAO, VBO, EBO;
public:
    SphereGridRenderer(OpenGLContext context, Shader&& shader_, SphereGridMesh&& mesh_);
    SphereGridRenderer(const SphereGridRenderer&) = delete;
    SphereGridRenderer& operator=(const SphereGridRenderer&) = delete;
    SphereGridRenderer(SphereGridRenderer&& other) noexcept;
    SphereGridRenderer& operator=(SphereGridRenderer&& other) noexcept;
    ~SphereGridRenderer();

    void render(float radius);
};