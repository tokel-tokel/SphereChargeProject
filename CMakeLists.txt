cmake_minimum_required(VERSION 3.31)
project(SphereChargeProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_GLAD "Build GLAD from source" ON)

include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

add_executable(${PROJECT_NAME} src/main.cpp
        src/graphics/Shader.cpp
        src/graphics/SphereRenderer.cpp
        src/graphics/MainWindow.cpp
        src/graphics/GLFWContext.cpp
        src/graphics/OpenGLContext.cpp
        src/graphics/SphereGridRenderer.cpp
        src/graphics/Camera.cpp
        src/ConsoleInput.cpp
        src/ParrotDispatcher.cpp
)

add_library(glad STATIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad.c)
target_include_directories(glad PUBLIC include)

target_link_libraries(${PROJECT_NAME}
        glfw
        OpenGL::OpenGL
        glad
)

target_include_directories(${PROJECT_NAME} PRIVATE ${glfw_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(${PROJECT_NAME}
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
else()
    install(
            DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/
            DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
    )
endif()
