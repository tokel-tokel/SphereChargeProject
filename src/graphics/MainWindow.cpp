#include <glad/glad.h>
#include "MainWindow.h"

#include <iostream>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include "GLFWContext.h"

MainWindow::MainWindow(unsigned short width, unsigned short height) : width(width), height(height)
{
    camera.setAspectRatio(static_cast<float>(width) / height);
    camera.setPosition({0.0f, 0.0f, 2.0f});
    GLFWContext::init();
    window = glfwCreateWindow(width, height, "Charged Sphere", nullptr, nullptr);
    if (!window) throw std::runtime_error("Failed to create GLFW window");
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwDestroyWindow(window);
        throw std::runtime_error("Failed to initialize GLAD");
    }
    dispatcher.emplace(*this);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, &MainWindow::frameBufferSizeCallback);
    glfwSetKeyCallback(window, &WindowInputDispatcher::keyCallback);
    glfwSetCursorPosCallback(window, &WindowInputDispatcher::mouseCallback);
    glfwSetScrollCallback(window, &WindowInputDispatcher::scrollCallback);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
}

MainWindow::MainWindow(MainWindow&& other) noexcept : width(other.width), height(other.height), window(other.window), sphere(std::move(other.sphere)),
    renderer(std::move(other.renderer)), gridRenderer(std::move(other.gridRenderer)), focusRenderer(std::move(other.focusRenderer))
{
    other.width = 0;
    other.height = 0;
    other.window = nullptr;
}

MainWindow& MainWindow::operator=(MainWindow&& other) noexcept
{
    if (this != &other)
    {
        glfwDestroyWindow(window);
        width = other.width;
        height = other.height;
        window = other.window;
        sphere = std::move(other.sphere);
        camera = other.camera;
        renderer = std::move(other.renderer);
        gridRenderer = std::move(other.gridRenderer);
        focusRenderer = std::move(other.focusRenderer);
        other.width = 0;
        other.height = 0;
        other.window = nullptr;
    }
    return *this;
}

MainWindow::~MainWindow()
{
    glfwDestroyWindow(window);
}

void MainWindow::run()
{
    timer.reset();
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(.8f, .8f, .8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer->render(.8f, camera);
        gridRenderer->render(.8f, camera);
        focusRenderer->render();
        glfwSwapBuffers(window);
        glfwPollEvents();
        dispatcher->keyUpdate();

        timer.update();
    }
}

OpenGLContext MainWindow::getContext()
{
    return OpenGLContext(window);
}

void MainWindow::setRenderer(SphereRendererBuilder& builder)
{
    renderer.emplace(builder.setReference(sphere).build());
}

void MainWindow::setGridRenderer(SphereGridRenderer&& renderer_)
{
    gridRenderer.emplace(std::move(renderer_));
}

void MainWindow::setFocusRenderer(FocusRenderer&& renderer_)
{
    focusRenderer.emplace(std::move(renderer_));
    focusRenderer->setAspectRatio(static_cast<float>(width) / height);
}

void MainWindow::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    auto thisPtr{static_cast<MainWindow*>(glfwGetWindowUserPointer(window))};
    thisPtr->width = width;
    thisPtr->height = height;
    auto aspect{static_cast<float>(width) / height};
    thisPtr->camera.setAspectRatio(aspect);
    thisPtr->focusRenderer->setAspectRatio(aspect);
}
