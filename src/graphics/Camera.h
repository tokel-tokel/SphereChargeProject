#pragma once
#include <glm/glm.hpp>
#include "glm/ext/scalar_constants.hpp"


class Camera
{
private:
    static constexpr float pi{glm::pi<float>()};
    static constexpr glm::vec3 globalUp{0.0f, 1.0f, 0.0f};
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    float azimuth{pi}, polar{pi / 2};
    mutable glm::vec3 forward{}, right{}, up{};
    mutable bool vecInit{false};
    float fov{pi / 3};
    float nearPlane{0.1f};
    float farPlane{1000.0f};
    float aspectRatio{4.0f / 3.0f};

public:
    Camera() = default;
    Camera(glm::vec3 positon_, float azimuth_, float polar_, float fow_, float aspect);
    void setPosition(glm::vec3 position_);
    void setAzimuth(float azimuth_);
    void setPolar(float polar_);
    void setFov(float fov_);
    void centralize();
    void rotate(float dAzimuth, float dPolar);
    void move(float dForward, float dRight);
    void moveUp(float dUp);
    void scale(float dFov);
    void setAspectRatio(float aspect);
    glm::mat4 getView() const;
    glm::mat4 getProjection() const;

private:
    void lazyVecInit() const;
};

