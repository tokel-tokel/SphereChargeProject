#pragma once
#include "Camera.h"
#include "SphereRenderer.h"
#include "OpenGLContext.h"
#include "SphereGridRenderer.h"
#include "Timer.h"
#include "GLFW/glfw3.h"
#include "objects/ChargedSphere.h"
#include "WindowInputDispatcher.h"
class WindowInputDispatcher;

class MainWindow 
{
    friend class WindowInputDispatcher;
private:
    unsigned short width;
    unsigned short height;
    GLFWwindow* window{nullptr};
    Timer timer;
    ChargedSphere sphere;
    Camera camera;
    std::optional<WindowInputDispatcher> dispatcher;
    std::optional<SphereRenderer> renderer; // инициализируем через builder
    std::optional<SphereGridRenderer> gridRenderer;
public:
    MainWindow(unsigned short width, unsigned short height);
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&& other) noexcept;
    MainWindow& operator=(MainWindow&& other) noexcept;
    ~MainWindow();
    OpenGLContext getContext();
    void setRenderer(SphereRendererBuilder& builder);
    void setGridRenderer(SphereGridRenderer&& renderer_);
    void run();
private:
    void processInput();
    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};
