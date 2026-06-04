#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "ball_object.hpp"

class Grid
{
public:
    Grid(float cellSize);
    ~Grid();
    void updateGrid(const std::vector<ballObject*>& balls, float warpStrength);
    void draw(Shader &shaderProgram) const;

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
    int gridWidth, gridHeight, gridDepth;
    float cellSize;
    std::vector<glm::vec3> originalPositions;

    void createGrid();
    void updateVertexBuffer();
};