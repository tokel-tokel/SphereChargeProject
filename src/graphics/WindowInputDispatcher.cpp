#include <glad/glad.h>
#include "WindowInputDispatcher.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "MainWindow.h"

void IKeyStrategy::setNext(std::unique_ptr<IKeyStrategy>&& next_)
{
    next = std::move(next_);
}

WindowInputDispatcher::WindowInputDispatcher(MainWindow& window) : mainWindow(window),
    defaultControls(createDefaultControls(window.camera, window.timer, moveVelocity))
{
    currentStrategy = defaultControls;
}

void WindowInputDispatcher::keyUpdate() const
{
    currentStrategy->update();
}


void WindowInputDispatcher::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto mWindow{static_cast<MainWindow*>(glfwGetWindowUserPointer(window))};
    auto& thisRef{mWindow->dispatcher.value()};
    thisRef.currentStrategy->onKey(key, action);
}


template<int Key, typename Action>
class KeyHandler : public IKeyStrategy {
    Action action;
    bool pressed{false};
public:
    KeyHandler(Action action) : action(std::move(action)) {}

    void onKey(int key, int act) override {
        if (key != Key)
        {
            if (next) next->onKey(key, act);
            return;
        }
        if (act == GLFW_PRESS && !pressed) pressed = true;
        else if (act == GLFW_RELEASE && pressed) pressed = false;
    }

    void update() override
    {
        if (pressed) action();
        if (next) next->update();
    }
};

template<int Key, typename Action>
std::unique_ptr<IKeyStrategy> makeHandler(Action action) {
    return std::make_unique<KeyHandler<Key, Action>>(std::move(action));
}

std::unique_ptr<IKeyStrategy> WindowInputDispatcher::createDefaultControls(Camera& camera, const Timer& timer, const float& velocity)
{
    std::unique_ptr aStrategy{ makeHandler<GLFW_KEY_A>([&]
    {
        camera.move(0.0f, -velocity * timer.getDelta());
    }) };
    std::unique_ptr dStrategy{ makeHandler<GLFW_KEY_D>([&]
    {
        camera.move(0.0f, velocity * timer.getDelta());
    }) };
    std::unique_ptr sStrategy{ makeHandler<GLFW_KEY_S>([&]
    {
        camera.move(-velocity * timer.getDelta(), 0.0f);
    }) };
    std::unique_ptr wStrategy{ makeHandler<GLFW_KEY_W>([&]
    {
        camera.move(velocity * timer.getDelta(), 0.0f);
    }) };

    dStrategy->setNext(std::move(aStrategy));
    sStrategy->setNext(std::move(dStrategy));
    wStrategy->setNext(std::move(sStrategy));
    return wStrategy;
}