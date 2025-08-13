#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "OpenGLContext.h"
#include "RendererBase.h"
#include "Shader.h"
#include "glm/ext/scalar_constants.hpp"

struct SphereGridMesh : Mesh<2, 2>
{
    static SphereGridMesh createGrid(GLuint gmeridians, GLuint gparallels, GLuint smeridians, GLuint sparallels);
private:
    static constexpr float pi{glm::pi<float>()};
    static constexpr GLuint ceilDiv(GLuint n, GLuint m)
    {
        return (n + m - 1) / m;
    }
};

class SphereGridRenderer : RendererBase<SphereGridMesh>
{
public:
    SphereGridRenderer(OpenGLContext context, Shader&& shader_, SphereGridMesh&& mesh_);
    SphereGridRenderer(const SphereGridRenderer&) = delete;
    SphereGridRenderer& operator=(const SphereGridRenderer&) = delete;
    SphereGridRenderer(SphereGridRenderer&&) = default;
    SphereGridRenderer& operator=(SphereGridRenderer&&) = default;
    ~SphereGridRenderer() = default;

    void render(float radius, const Camera& camera);
};