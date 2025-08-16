#include "ChargedSphere.h"

#include <iostream>

#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/constants.hpp"

ChargedSphere::ChargedSphere()
{
    solidPosition = {glm::pi<float>() / 6, glm::pi<float>() / 3};
    solidAngle = glm::pi<float>();
    calcConeCos();
}

float ChargedSphere::getConeCos() const
{
    return coneCos;
}

glm::vec2 ChargedSphere::getSolidPosition() const
{
    return solidPosition;
}


void ChargedSphere::calcConeCos()
{
    coneCos = 1 - solidAngle / glm::two_pi<float>(); // versin(theta) = 1 - cos(theta) = solid / 2pi -> haversin(theta) = solid / 2pi / 2
}


