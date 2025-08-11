#include "graphics/MainWindow.h"
#include "graphics/SphereRenderer.h"

int main()
{
    MainWindow mainWindow(800, 600);
    SphereRendererBuilder builder;
    builder.setContext(mainWindow.getContext()).setMesh(20, 12).
        setShader(Shader::loadFromFile("resouces/shaders/sphere_vertex.glsl", "resources/shaders/sphere_fragment.glsl", mainWindow.getContext()).value());
    mainWindow.setRenderer(builder);
    mainWindow.run();
}

