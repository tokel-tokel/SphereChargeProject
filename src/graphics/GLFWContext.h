#pragma once

class GLFWContext 
{
public:
    static GLFWContext& init();
    ~GLFWContext();
private:
    GLFWContext();
};
