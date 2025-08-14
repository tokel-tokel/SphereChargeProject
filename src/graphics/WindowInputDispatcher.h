#pragma once
#include <memory>
#include "Timer.h"
#include "Camera.h"
#include "GLFW/glfw3.h"

class MainWindow;

class IKeyStrategy
{
protected:
    std::unique_ptr<IKeyStrategy> next;
public:
    virtual void onKey(int key, int act) = 0;
    virtual void update() = 0;
    virtual void setNext(std::unique_ptr<IKeyStrategy>&& next_);
    virtual ~IKeyStrategy() = default;
};

class WindowInputDispatcher
{
private:
    float moveVelocity{2.0f};
    MainWindow& mainWindow;
    std::shared_ptr<IKeyStrategy> defaultControls;
    std::shared_ptr<IKeyStrategy> currentStrategy;
public:
    WindowInputDispatcher(MainWindow& window);
    void keyUpdate() const;
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
    static std::unique_ptr<IKeyStrategy> createDefaultControls(Camera& camera, const Timer& timer, const float& velocity);
};