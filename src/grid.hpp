#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "ball_object.hpp"

class Grid
{
public:
    Grid(int width, int height, float cellSize);
    ~Grid();
    void updateGrid();
    void draw(Shader &shaderProgram) const;

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
    int gridWidth, gridHeight;
    float cellSize;
    std::vector<glm::vec2> originalPosition;

    void createGrid();
    void updateVertexBuffer();
};