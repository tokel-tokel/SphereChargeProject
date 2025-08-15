#include <glad/glad.h>
#include "WindowInputDispatcher.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "MainWindow.h"

void IKeyStrategy::setNext(std::unique_ptr<IKeyStrategy>&& next_)
{
    next = std::move(next_);
}

class EscKeyStrategy : public IKeyStrategy
{
private:
    bool onMenu{false};
    WindowInputDispatcher& dispatcher;
public:
    explicit EscKeyStrategy(WindowInputDispatcher& dispatcher) : dispatcher(dispatcher) {}
    void onKey(int key, int act) override
    {
        using enum WindowInputDispatcher::MouseMode;
        if (key == GLFW_KEY_ESCAPE && act == GLFW_PRESS)
        {
            onMenu = !onMenu;
            dispatcher.setMouseStrategy(onMenu ? Menu : Camera);
        }
        else if (next) next->onKey(key, act);
    }
    void update() override
    {
        if (next) next->update();
    }
};

CameraMouseStrategy::CameraMouseStrategy(MainWindow& mainWindow, Camera& camera, const float& sensivity, const float& zoomSpeed) :
    mainWindow(mainWindow), camera(camera), sensivity(sensivity), zoomSpeed(zoomSpeed) {}

void CameraMouseStrategy::onMove(double xpos, double ypos)
{
    if (firstMove)
    {
        lastX = xpos;
        lastY = ypos;
        firstMove = false;
        return;
    }
    auto dx{static_cast<float>(xpos - lastX)};
    auto dy{static_cast<float>(ypos - lastY)};
    lastX = xpos;
    lastY = ypos;
    camera.rotate(-dx * sensivity, dy * sensivity);
}

void CameraMouseStrategy::onScroll(double yoffset)
{
    camera.scale(yoffset * zoomSpeed);
}


void CameraMouseStrategy::init()
{
    glfwSetInputMode(mainWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) glfwSetInputMode(mainWindow.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    firstMove = true;
}

MenuMouseStrategy::MenuMouseStrategy(MainWindow& mainWindow) : mainWindow(mainWindow) {}

void MenuMouseStrategy::onMove(double xpos, double ypos) {}

void MenuMouseStrategy::onScroll(double yoffset) {}


void MenuMouseStrategy::init()
{
    glfwSetInputMode(mainWindow.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(mainWindow.window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
}

WindowInputDispatcher::WindowInputDispatcher(MainWindow& window) : mainWindow(window)
{
    defaultControls = std::make_shared<EscKeyStrategy>(*this);
    defaultControls->setNext(createDefaultControls(mainWindow.camera, mainWindow.timer, moveVelocity));
    currentStrategy = defaultControls;
    cameraControls = std::make_shared<CameraMouseStrategy>(mainWindow, mainWindow.camera, sensivity, zoomSpeed);
    menuControls = std::make_shared<MenuMouseStrategy>(mainWindow);
    currentMouseStrategy = cameraControls;
    currentMouseStrategy->init();
}

void WindowInputDispatcher::setMouseStrategy(MouseMode mode)
{
    if (mode == MouseMode::Camera) currentMouseStrategy = cameraControls;
    else if (mode == MouseMode::Menu) currentMouseStrategy = menuControls;
    else return;
    currentMouseStrategy->init();
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

void WindowInputDispatcher::mouseCallback(GLFWwindow* window, double x, double y)
{
    auto mWindow{static_cast<MainWindow*>(glfwGetWindowUserPointer(window))};
    auto& thisRef{mWindow->dispatcher.value()};
    thisRef.currentMouseStrategy->onMove(x, y);
}

void WindowInputDispatcher::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto mWindow{static_cast<MainWindow*>(glfwGetWindowUserPointer(window))};
    auto& thisRef{mWindow->dispatcher.value()};
    thisRef.currentMouseStrategy->onScroll(yoffset);
}


template<int Key, typename Action>
class KeyHandler : public IKeyStrategy {
    Action action;
    bool pressed{false};
public:
    KeyHandler(Action action) : action(std::move(action)) {}

    void onKey(int key, int act) override
    {
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

template<int Key, int Act, typename Action>
class ActKeyHandler : public IKeyStrategy {
    Action action;
public:
    ActKeyHandler(Action action) : action(std::move(action)) {}

    void onKey(int key, int act) override
    {
        if (key == Key && act == Act) action();
        else if (next) next->onKey(key, act);
    }

    void update() override
    {
        if (next) next->update();
    }
};

template<int Key, typename Action>
std::unique_ptr<IKeyStrategy> makeHandler(Action action) {
    return std::make_unique<KeyHandler<Key, Action>>(std::move(action));
}

template<int Key, int Act, typename Action>
std::unique_ptr<IKeyStrategy> makeHandler(Action action) {
    return std::make_unique<ActKeyHandler<Key, Act, Action>>(std::move(action));
}

std::unique_ptr<IKeyStrategy> WindowInputDispatcher::createDefaultControls(Camera& camera, const Timer& timer, const float& velocity)
{
    std::unique_ptr fStrategy{ makeHandler<GLFW_KEY_F, GLFW_PRESS>([&]
    {
        camera.centralize();
    }) };
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
    std::unique_ptr spaceStrategy{ makeHandler<GLFW_KEY_SPACE>([&]
    {
        camera.moveUp(velocity * timer.getDelta());
    }) };
    std::unique_ptr lshiftStrategy{ makeHandler<GLFW_KEY_LEFT_SHIFT>([&]
    {
        camera.moveUp(-velocity * timer.getDelta());
    }) };


    aStrategy->setNext(std::move(fStrategy));
    dStrategy->setNext(std::move(aStrategy));
    sStrategy->setNext(std::move(dStrategy));
    wStrategy->setNext(std::move(sStrategy));
    spaceStrategy->setNext(std::move(wStrategy));
    lshiftStrategy->setNext(std::move(spaceStrategy));
    return lshiftStrategy;
}