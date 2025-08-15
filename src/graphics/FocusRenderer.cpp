#include "FocusRenderer.h"

FocusRenderer::FocusRenderer(OpenGLContext context, Shader&& shader_) :
    RendererBase(context, std::move(shader_), std::move(genMesh()))
{
    shader.addUniformLocation("aspect");
}

void FocusRenderer::setAspectRatio(float aspect)
{
    if (aspect <= 0.0f) aspectRatio = 0.01f;
    else aspectRatio = aspect;
}

void FocusRenderer::render()
{
    shader.use();
    shader.setUniform("aspect", aspectRatio);
    context.makeCurrent();
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, mesh.vertices.size());
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    shader.unuse();
}


Mesh<2, 2> FocusRenderer::genMesh()
{
    Mesh<2, 2> mesh;
    constexpr float size{0.04f};
    mesh.vertices.insert(mesh.vertices.end(), {size, size, -size, -size, -size, size, size, -size});
    return mesh;
}

