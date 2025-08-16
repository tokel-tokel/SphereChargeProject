#pragma once
#include <glm/glm.hpp>

class ChargedSphere 
{
private:
    float solidAngle;
    glm::vec2 solidPosition;
    float coneCos;
public:
    ChargedSphere();
    [[nodiscard]] float getConeCos() const;
    [[nodiscard]] glm::vec2 getSolidPosition() const;
private:
    void calcConeCos();
};
