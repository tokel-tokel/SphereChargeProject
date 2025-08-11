#pragma once
#include "OpenGLContext.h"
#include "SphereRenderer.h"
#include "GLFW/glfw3.h"
#include "objects/ChargedSphere.h"

class MainWindow 
{
private:
    unsigned short width;
    unsigned short height;
    GLFWwindow* window{nullptr};
    ChargedSphere sphere;
    std::optional<SphereRenderer> renderer; // инициализируем через builder
public:
    MainWindow(unsigned short width, unsigned short height);
    ~MainWindow();
    OpenGLContext getContext();
    void setRenderer(SphereRendererBuilder& builder);
    void run();
private:
    void processInput();
    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};
