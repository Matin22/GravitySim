#pragma once

#include <glm/glm.hpp>
#include <GLAD/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "conf.hpp"

class Crosshair
{
private:
    unsigned int VAO, VBO;
    float size;

public:
    Crosshair(float size);
    ~Crosshair();
    void draw(Shader &shader);
};