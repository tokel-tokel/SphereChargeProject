#pragma once
#include <memory>
#include "Timer.h"
#include "Camera.h"
#include "GLFW/glfw3.h"

class MainWindow;

class IKeyStrategy
{
protected:
    std::shared_ptr<IKeyStrategy> next;
public:
    virtual void onKey(int key, int act) = 0;
    virtual void update() = 0;
    virtual void setNext(const std::shared_ptr<IKeyStrategy>& next_);
    virtual ~IKeyStrategy() = default;
};

class IMouseStrategy
{
public:
    virtual void onMove(double xpos, double ypos) = 0;
    virtual void onScroll(double yoffset) = 0;
    virtual void init() = 0;
    virtual ~IMouseStrategy() = default;
};

class CameraMouseStrategy : public IMouseStrategy
{
private:
    MainWindow& mainWindow;
    Camera& camera;
    const float& sensivity;
    const float& zoomSpeed;
    double lastX{0.0}, lastY{0.0};
    bool firstMove{true};
public:
    explicit CameraMouseStrategy(MainWindow& mainWindow, Camera& camera, const float& sensivity, const float& zoomSpeed);
    void onMove(double xpos, double ypos) override;
    void onScroll(double yoffset) override;
    void init() override;
};

class MenuMouseStrategy : public IMouseStrategy
{
private:
    MainWindow& mainWindow;
public:
    MenuMouseStrategy(MainWindow& mainWindow);
    void onMove(double xpos, double ypos) override;
    void onScroll(double yoffset) override;
    void init() override;
};

class WindowInputDispatcher
{
public:
    enum class MouseMode {Menu, Camera};
    enum class KeyMode {Menu, FreeCam};
private:
    float moveVelocity{4.0f};
    float sensivity{0.005};
    float zoomSpeed{0.08f};
    MainWindow& mainWindow;
    std::shared_ptr<IKeyStrategy> escControls;
    std::shared_ptr<IKeyStrategy> defaultControls;
    std::shared_ptr<IKeyStrategy> menuKeyControls;
    std::shared_ptr<IKeyStrategy> currentStrategy;
    std::shared_ptr<IMouseStrategy> cameraControls;
    std::shared_ptr<IMouseStrategy> menuControls;
    std::shared_ptr<IMouseStrategy> currentMouseStrategy;
public:
    WindowInputDispatcher(MainWindow& window);
    void setMouseStrategy(MouseMode mode);
    void setKeyStrategy(KeyMode mode);
    void keyUpdate() const;
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, double x, double y);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
private:
    static std::unique_ptr<IKeyStrategy> createDefaultControls(Camera& camera, const Timer& timer, const float& velocity);
};