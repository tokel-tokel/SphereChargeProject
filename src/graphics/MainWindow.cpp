#include "MainWindow.h"

#include <stdexcept>

#include "GLFWContext.h"

MainWindow::MainWindow(unsigned short width, unsigned short height) : width(width), height(height)
{
    GLFWContext::init();
    window = glfwCreateWindow(width, height, "Charged Sphere", nullptr, nullptr);
    if (!window) throw std::runtime_error("Failed to create GLFW window");
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwDestroyWindow(window);
        throw std::runtime_error("Failed to initialize GLAD");
    }
    glfwSetFramebufferSizeCallback(window, &MainWindow::frameBufferSizeCallback);
}

MainWindow::~MainWindow()
{
    glfwDestroyWindow(window);
}

void MainWindow::run()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
        processInput();
        glClearColor(.8f, .8f, .8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer->render(.8f);
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

void MainWindow::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void MainWindow::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
