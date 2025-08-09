#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "ConsoleInput.h"
#include "ParrotDispatcher.h"

int main()
{
    std::atomic<bool> exitFlag{false};
    auto inputDispatcher{std::make_shared<ParrotDispatcher>(exitFlag)};
    ConsoleInput::getInstance().setDispatcher(inputDispatcher);

    while (!exitFlag);
}

