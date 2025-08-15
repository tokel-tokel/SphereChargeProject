#pragma once
#include "Mesh.h"
#include "RendererBase.h"


class FocusRenderer : RendererBase<Mesh<2, 2>>
{
private:
    float aspectRatio{1.0f};
public:
    FocusRenderer(OpenGLContext context, Shader&& shader_);
    void setAspectRatio(float aspect);
    void render();
private:
    static Mesh<2, 2> genMesh();
};
