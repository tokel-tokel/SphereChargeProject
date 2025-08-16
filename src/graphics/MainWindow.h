#pragma once
#include <functional>

#include "Camera.h"
#include "FocusRenderer.h"
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
    friend class CameraMouseStrategy;
    friend class MenuMouseStrategy;
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
    std::optional<FocusRenderer> focusRenderer;
    std::function<void()> freeCamRender;
    std::function<void()> menuRender;
    std::function<void()> currentRender;
public:
    enum class RenderMode {Menu, FreeCam};
    MainWindow(unsigned short width, unsigned short height);
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&& other) noexcept;
    MainWindow& operator=(MainWindow&& other) noexcept;
    ~MainWindow();
    OpenGLContext getContext();
    void setRenderer(SphereRendererBuilder& builder);
    void setGridRenderer(SphereGridRenderer&& renderer_);
    void setFocusRenderer(FocusRenderer&& renderer_);
    void switchRender(RenderMode mode);
    void run();
private:
    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};
