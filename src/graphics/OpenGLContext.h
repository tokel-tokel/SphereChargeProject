#pragma once
#include "GLFW/glfw3.h"


class OpenGLContext 
{
private:
    GLFWwindow* window;
public:
    explicit OpenGLContext(GLFWwindow* window);
    void makeCurrent() const;
};
