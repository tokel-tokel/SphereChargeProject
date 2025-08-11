#include "OpenGLContext.h"

OpenGLContext::OpenGLContext(GLFWwindow* window) : window{window} {}

void OpenGLContext::makeCurrent() const
{
    glfwMakeContextCurrent(window);
}