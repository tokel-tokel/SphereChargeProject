#include "SphereGridRenderer.h"

#include <stdexcept>

SphereGridMesh SphereGridMesh::createGrid(GLuint meridians, GLuint parallels)
{
    if (meridians < 2 || parallels < 3) throw std::invalid_argument("Count of meridians must be at least 2 and of parallels at least 3");
    const GLuint merSize{parallels - 2};
    SphereGridMesh mesh;
    mesh.vertices.insert(mesh.vertices.end(), {0.0f, 0.0f}); // North Pole
    mesh.vertices.insert(mesh.vertices.end(), {0.0f, pi}); // South Pole
    constexpr GLuint nPole{0};
    constexpr GLuint sPole{1};
    constexpr GLuint begin{2};
    for (GLuint mer{0}; mer < meridians; ++mer)
    {
        float phi{mer * 2 * pi / static_cast<float>(meridians)};
        for (GLuint par{1}; par < parallels - 1; ++par)
        {
            float theta{par * pi / static_cast<float>(parallels - 1)};
            mesh.vertices.insert(mesh.vertices.end(), {phi, theta});
            mesh.indices.insert(mesh.indices.end(), {(begin + mer * merSize + par - 1),
                        (begin + (mer + 1) % meridians * merSize + par - 1)}); // parallel line
            if (par == 1) // meridian line
            {
                mesh.indices.insert(mesh.indices.end(), {nPole,
                        (begin + mer * merSize + par - 1)});
            }
            else if (par > 1 && par < parallels - 2)
            {
                mesh.indices.insert(mesh.indices.end(), {(begin + mer * merSize + par - 1),
                    (begin + mer * merSize + par)});
            }
            else if (par == parallels - 2)
            {
                mesh.indices.insert(mesh.indices.end(), {(begin + mer * merSize + par - 1),
                        sPole});
            }
        }
    }
    return mesh;
}

SphereGridRenderer::SphereGridRenderer(OpenGLContext context, Shader&& shader_, SphereGridMesh&& mesh_)
{
    shader.addUniformLocation("radius");
    context.makeCurrent();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), mesh.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, mesh.vertSize * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SphereGridRenderer::SphereGridRenderer(SphereGridRenderer&& other) noexcept : context(other.context), shader(std::move(other.shader)),
    mesh(std::move(other.mesh)), VAO(other.VAO), VBO(other.VBO), EBO(other.EBO)
{
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

SphereGridRenderer& SphereGridRenderer::operator=(SphereGridRenderer&& other) noexcept
{
    if (this != &other)
    {
        this->~SphereGridRenderer();
        context = other.context;
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

SphereGridRenderer::~SphereGridRenderer()
{
    if(VAO != 0)
    {
        context.makeCurrent();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

void SphereGridRenderer::render(float radius)
{
}
