#include "graphics/MainWindow.h"
#include "graphics/SphereRenderer.h"

int main()
{
    MainWindow mainWindow(800, 600);
    SphereRendererBuilder builder;
    builder.setContext(mainWindow.getContext()).setMesh(100, 60).
        setShader(Shader::loadFromFile("resources/shaders/sphere_vertex.glsl", "resources/shaders/sphere_fragment.glsl", mainWindow.getContext()).value());
    mainWindow.setRenderer(builder);
    Shader gridShader{Shader::loadFromFile("resources/shaders/sphere_grid_vertex.glsl", "resources/shaders/sphere_grid_fragment.glsl",
        mainWindow.getContext()).value()};
    mainWindow.setGridRenderer(SphereGridRenderer(mainWindow.getContext(), std::move(gridShader),
        SphereGridMesh::createGrid(20, 11, 100, 60)));
    mainWindow.setFocusRenderer(FocusRenderer(mainWindow.getContext(),
        Shader::loadFromFile("resources/shaders/focus_vertex.glsl", "resources/shaders/focus_fragment.glsl", mainWindow.getContext()).value()));
    mainWindow.run();
}

