#include <glad/glad.h>
#include "SphereRenderer.h"

#include <array>
#include <iostream>
#include <numeric>
#include <stdexcept>

SphereMesh SphereMesh::createSphere(GLuint meridians, GLuint parallels) // TODO: надо сделать так чтобы разрешение мэша сетки соответствовало разрешению сферы, но рисовать только ограниченное число меридианов и параллелей
{
    if (meridians < 2 || parallels < 3) throw std::invalid_argument("Count of meridians must be at least 2 and of parallels at least 3");
    const GLuint merSize{parallels - 2};
    SphereMesh mesh;
    mesh.vertices.insert(mesh.vertices.end(), {0.0f, 0.0f, 0.0f, 1.0f, 0.0f}); // North Pole
    mesh.vertices.insert(mesh.vertices.end(), {0.0f, pi, 0.0f, -1.0f, 0.0f}); // South Pole
    constexpr GLuint nPole{0};
    constexpr GLuint sPole{1};
    constexpr GLuint begin{2};
    for (GLuint mer{0}; mer < meridians; ++mer)
    {
        float phi{mer * 2 * pi / static_cast<float>(meridians)};
        for (GLuint par{1}; par < parallels - 1; ++par)
        {
            float theta{calcTheta(par, parallels)};
            mesh.vertices.insert(mesh.vertices.end(), {phi, theta,
                glm::sin(phi)*glm::sin(theta), glm::cos(theta), glm::cos(phi)*glm::sin(theta)});
            if (par > 1 && par < parallels - 1)
            {
                mesh.indices.insert(mesh.indices.end(), {(begin + mer * merSize + par - 1),
                    (begin + (mer + 1) % meridians * merSize + par - 2),
                    (begin + mer * merSize + par - 2)});
                mesh.indices.insert(mesh.indices.end(), {(begin + mer * merSize + par - 1),
                    (begin + (mer + 1) % meridians * merSize + par - 1),
                    (begin + (mer + 1) % meridians * merSize + par - 2)});
            }
            if (par == 1)
            {
                mesh.indices.insert(mesh.indices.end(), {(begin + mer * merSize + par - 1),
                        (begin + (mer + 1) % meridians * merSize + par - 1),
                        nPole});
            }
            if (par == parallels - 2)
            {
                mesh.indices.insert(mesh.indices.end(), {(begin + mer * merSize + par - 1),
                        sPole,
                        (begin + (mer + 1) % meridians * merSize + par - 1)});
            }
        }
    }
    return mesh;
}

SphereRenderer::SphereRenderer(OpenGLContext context, ChargedSphere& sphere_, Shader&& shader_, SphereMesh&& mesh_) :
    RendererBase(context, std::move(shader_), std::move(mesh_)), sphere(sphere_)
{
    shader.addUniformLocation("radius");
    shader.addUniformLocation("view");
    shader.addUniformLocation("projection");
}

void SphereRenderer::render(float radius, const Camera& camera) const
{
    shader.use();
    shader.setUniform("radius", radius);
    shader.setUniform("view", camera.getView());
    shader.setUniform("projection", camera.getProjection());
    context.makeCurrent();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);

    GLenum error{glGetError()};
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error after draw: " << error << "\n";
        while((error = glGetError()) != GL_NO_ERROR) std::cout << "Additional error: " << error << "\n";
    }
    glBindVertexArray(0);
    shader.unuse();
}


SphereRendererBuilder& SphereRendererBuilder::setMesh(GLuint meridians, GLuint parallels)
{
    mesh = SphereMesh::createSphere(meridians, parallels);
    return *this;
}

SphereRendererBuilder& SphereRendererBuilder::setContext(OpenGLContext context_)
{
    context.emplace(context_);
    return *this;
}

SphereRendererBuilder& SphereRendererBuilder::setShader(Shader&& shader_)
{
    shader.emplace(std::move(shader_));
    return *this;
}

SphereRendererBuilder& SphereRendererBuilder::setReference(ChargedSphere& sphere_)
{
    sphere.emplace(sphere_);
    return *this;
}

SphereRenderer SphereRendererBuilder::build()
{
    if (!sphere || !shader) throw std::runtime_error("Empty builder params");
    return SphereRenderer(*context,sphere->get(), std::move(*shader), std::move(mesh));
}

