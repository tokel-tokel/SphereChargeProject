#include "graphics/MainWindow.h"
#include "graphics/SphereRenderer.h"

int main()
{
    MainWindow mainWindow(800, 600);
    SphereRendererBuilder builder;
    builder.setContext(mainWindow.getContext()).setMesh(1000, 800).
        setShader(Shader::loadFromFile("resources/shaders/sphere_vertex.glsl", "resources/shaders/sphere_fragment.glsl", mainWindow.getContext()).value());
    mainWindow.setRenderer(builder);
    mainWindow.run();
}

