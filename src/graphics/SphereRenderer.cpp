#include <glad/glad.h>
#include "SphereRenderer.h"

#include <array>
#include <iostream>
#include <numeric>
#include <stdexcept>

SphereMesh SphereMesh::createSphere(GLuint meridians, GLuint parallels)
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

static_assert(std::accumulate(SphereMesh::inParamSize.begin(), SphereMesh::inParamSize.end(), 0) == SphereMesh::vertSize, "Incorrect layout");

SphereRenderer::SphereRenderer(OpenGLContext context, ChargedSphere& sphere_, Shader&& shader_, SphereMesh&& mesh_) :context(context), sphere(sphere_), shader(std::move(shader_)),
    mesh(std::move(mesh_))
{
    shader.addUniformLocation("radius");
    context.makeCurrent();
    GLint attribLocation0 = glGetAttribLocation(shader.getId(), "position");
    GLint attribLocation1 = glGetAttribLocation(shader.getId(), "inNormal");
    std::cout << "Attribute locations - position: " << attribLocation0 << ", inNormal: " << attribLocation1 << std::endl;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), mesh.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, mesh.inParamSize[0], GL_FLOAT, GL_FALSE, mesh.vertSize * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, mesh.inParamSize[1], GL_FLOAT, GL_FALSE, mesh.vertSize * sizeof(float), (GLvoid*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SphereRenderer::SphereRenderer(SphereRenderer&& other) noexcept : context(other.context), sphere(other.sphere), shader(std::move(other.shader)),
    mesh(std::move(other.mesh)), VAO(other.VAO), VBO(other.VBO), EBO(other.EBO)
{
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

SphereRenderer& SphereRenderer::operator=(SphereRenderer&& other) noexcept
{
    if (this != &other)
    {
        this->~SphereRenderer();
        context = other.context;
        sphere = other.sphere;
        shader = std::move(other.shader);
        mesh = std::move(other.mesh);
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
    }
    return *this;
}

SphereRenderer::~SphereRenderer()
{
    std::cout << "Destroying SphereRenderer, VAO: " << VAO << std::endl;
    if(VAO != 0)
    {
        context.makeCurrent();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

void SphereRenderer::render(float radius) const
{
    context.makeCurrent();

    //debug
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    shader.use();
    shader.setUniform("radius", radius);
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

