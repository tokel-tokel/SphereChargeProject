#include <glad/glad.h>
#include "SphereGridRenderer.h"

#include <iostream>
#include <stdexcept>

SphereGridMesh SphereGridMesh::createGrid(GLuint gmeridians, GLuint gparallels, GLuint smeridians, GLuint sparallels)
{
    if (gmeridians < 2 || gparallels < 3 || smeridians < 2 || sparallels < 3) throw std::invalid_argument("Count of meridians must be at least 2 and of parallels at least 3");
    const GLuint parMutip{ceilDiv(sparallels - 1, gparallels - 1)};
    const GLuint merMultip{ceilDiv(smeridians, gmeridians)};
    sparallels = (gparallels - 1) * parMutip + 1;
    smeridians = gmeridians * merMultip;
    const GLuint fullMerSize{sparallels - 2};
    const GLuint partMerSize{gparallels - 2};
    SphereGridMesh mesh;
    mesh.vertices.insert(mesh.vertices.end(), {0.0f, 0.0f}); // North Pole
    mesh.vertices.insert(mesh.vertices.end(), {0.0f, pi}); // South Pole
    constexpr GLuint nPole{0};
    constexpr GLuint sPole{1};
    constexpr GLuint begin{2};

    auto calcIndex{[=](GLuint smer, GLuint gpar)
    {
        smer = smer % smeridians;
        GLuint fMers{ceilDiv(smer, merMultip)};
        GLuint index{fMers * fullMerSize + (smer - fMers) * partMerSize + begin}; // индекс начала текущего меридиана
        index += gpar * (1 + (parMutip - 1) * (smer % merMultip == 0)) - 1; // полный индекс
        return index;
    }};

    for (GLuint smer{0}; smer < smeridians; ++smer)
    {
        float phi{smer * 2 * pi / static_cast<float>(smeridians)};
        if (smer % merMultip == 0) // generate full meridian
        {
            GLuint merBegin{smer / merMultip * (fullMerSize + (merMultip - 1) * partMerSize) + begin};
            for (GLuint spar{1}; spar < sparallels - 1; ++spar)
            {
                float theta{spar * pi / static_cast<float>(sparallels - 1)};
                mesh.vertices.insert(mesh.vertices.end(), {phi, theta});
                if (spar == 1) mesh.indices.insert(mesh.indices.end(), {nPole, merBegin + spar - 1});
                if (spar < sparallels - 2) mesh.indices.insert(mesh.indices.end(), {merBegin + spar - 1, merBegin + spar});
                else mesh.indices.insert(mesh.indices.end(), {merBegin + spar - 1, sPole}); // соединение меридиана
                if (spar % parMutip == 0)
                {
                    mesh.indices.insert(mesh.indices.end(), {calcIndex(smer, spar / parMutip), calcIndex(smer + 1, spar / parMutip)}); // соединение параллели
                }

            } // generated fullMerSize verts
        }
        else //generate part meridian
        {
            for (GLuint gpar{1}; gpar < gparallels - 1; ++gpar)
            {
                float theta{gpar * pi / static_cast<float>(gparallels - 1)};
                mesh.vertices.insert(mesh.vertices.end(), {phi, theta});
                mesh.indices.insert(mesh.indices.end(), {calcIndex(smer, gpar), calcIndex(smer + 1, gpar)}); // соединение параллели
            } // generated partMerSize verts
        }
    }

    return mesh;
}

SphereGridRenderer::SphereGridRenderer(OpenGLContext context, Shader&& shader_, SphereGridMesh&& mesh_) :
    RendererBase(context, std::move(shader_), std::move(mesh_))
{
    shader.addUniformLocation("radius");
    shader.addUniformLocation("view");
    shader.addUniformLocation("projection");
}

void SphereGridRenderer::render(float radius, const Camera& camera)
{
    shader.use();
    shader.setUniform("radius", radius);
    shader.setUniform("view", camera.getView());
    shader.setUniform("projection", camera.getProjection());
    context.makeCurrent();
    glBindVertexArray(VAO);
    glDepthFunc(GL_LEQUAL);
    glDrawElements(GL_LINES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);

    GLenum error{glGetError()};
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error after draw grid: " << error << "\n";
        while((error = glGetError()) != GL_NO_ERROR) std::cout << "Additional error: " << error << "\n";
    }
    glDepthFunc(GL_LESS);
    glBindVertexArray(0);
    shader.unuse();
}
