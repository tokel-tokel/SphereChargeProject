#include "GLFWContext.h"

#include <stdexcept>
#include "GLFW/glfw3.h"

GLFWContext& GLFWContext::init()
{
    static GLFWContext context;
    return context;
}

GLFWContext::~GLFWContext()
{
    glfwTerminate();
}

GLFWContext::GLFWContext()
{
    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
