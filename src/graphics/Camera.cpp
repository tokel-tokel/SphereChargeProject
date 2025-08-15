#include "Camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"


Camera::Camera(glm::vec3 position_, float azimuth_, float polar_, float fov_, float aspect)
{
    setPosition(position_);
    setAzimuth(azimuth_);
    setPolar(polar_);
    setFov(fov_);
    setAspectRatio(aspect);
}

void Camera::setPosition(glm::vec3 position_)
{
    position = position_;
}

void Camera::setAzimuth(float azimuth_)
{
    vecInit = false;
    azimuth = glm::mod(azimuth_, 2 * pi);
    azimuth = azimuth < 0 ? azimuth + 2 * pi : azimuth;
}

void Camera::setPolar(float polar_)
{
    vecInit = false;
    polar = glm::clamp(polar_, pi / 36, pi * 35 / 36);
}

void Camera::setFov(float fov_)
{
    fov = glm::clamp(fov_, pi / 100, pi * 99 / 100);
}

void Camera::setAspectRatio(float aspect)
{
    if (aspect <= 0.0f) aspectRatio = 0.01f;
    else aspectRatio = aspect;
}

void Camera::centralize()
{
    glm::vec3 direction{glm::normalize(-position)};
    setPolar(glm::acos(direction.y));
    setAzimuth(glm::atan(direction.x, direction.z));
}



void Camera::rotate(float dAzimuth, float dPolar)
{
    setAzimuth(azimuth + dAzimuth);
    setPolar(polar + dPolar);
}

void Camera::move(float dForward, float dRight)
{
    lazyVecInit();
    position += dForward * forward + dRight * right;
}

void Camera::moveUp(float dUp)
{
    lazyVecInit();
    position += dUp * up;
}


void Camera::scale(float dFov)
{
    setFov(fov + dFov);
}

glm::mat4 Camera::getView() const
{
    lazyVecInit();
    return glm::lookAt(position, position + forward, {0.0f, 1.0f, 0.0f});
}

glm::mat4 Camera::getProjection() const
{
    return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::lazyVecInit() const
{
    if (vecInit) return;
    forward = {glm::sin(azimuth)*glm::sin(polar), glm::cos(polar), glm::cos(azimuth)*glm::sin(polar)};
    right = glm::normalize(glm::cross(forward, globalUp));
    up = glm::cross(right, forward);
    vecInit = true;
}
