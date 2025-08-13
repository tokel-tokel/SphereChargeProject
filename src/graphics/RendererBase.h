#pragma once
#include <glad/glad.h>
#include "OpenGLContext.h"
#include "Shader.h"

template <class MeshType>
class RendererBase
{
protected:
    OpenGLContext context;
    Shader shader;
    MeshType mesh;
    GLuint VAO, VBO, EBO;
public:
    RendererBase(OpenGLContext context, Shader&& shader_, MeshType&& mesh_) : context(context), shader(std::move(shader_)),
    mesh(std::move(mesh_))
    {
        context.makeCurrent();
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), mesh.indices.data(), GL_STATIC_DRAW);

        size_t offset{0};
        for (size_t i{0}; i < MeshType::layout.size(); ++i)
        {
            glVertexAttribPointer(i, MeshType::layout[i], GL_FLOAT, GL_FALSE, MeshType::vertSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
            glEnableVertexAttribArray(i);
            offset += MeshType::layout[i];
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    RendererBase(const RendererBase&) = delete;
    RendererBase& operator=(const RendererBase&) = delete;

    RendererBase(RendererBase&& other) noexcept : context(other.context), shader(std::move(other.shader)),
    mesh(std::move(other.mesh)), VAO(other.VAO), VBO(other.VBO), EBO(other.EBO)
    {
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
    }

    RendererBase& operator=(RendererBase&& other) noexcept
    {
        if (this != &other)
        {
            clear();
            context = other.context;
            shader = std::move(other.shader);
            mesh = std::move(other.mesh);
            VAO = other.VAO;
            VBO = other.VBO;
            EBO = other.EBO;
            other.EBO = other.VBO = other.VAO = 0;
        }
        return *this;
    }

    ~RendererBase()
    {
        clear();
    }

private:
    void clear() noexcept
    {
        if (VAO != 0)
        {
            context.makeCurrent();
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }
    }
};



