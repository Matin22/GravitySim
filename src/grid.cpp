#include <iostream>
#include <vector>

#include "grid.hpp"
#include "conf.hpp"

Grid::Grid(float cellSize)
    :gridWidth(conf::SCREEN_X / cellSize), gridHeight(conf::SCREEN_Y / cellSize), gridDepth(conf::SCREEN_X / cellSize),cellSize(cellSize)
{
    createGrid();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Grid::~Grid()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Grid::updateGrid(const std::vector<ballObject*>& balls, float warpStrength)
{
    auto get_index = [&](int x, int y, int z) {
        return static_cast<size_t>(
            z * (gridWidth + 1) * (gridHeight + 1) +
            y * (gridWidth + 1) +
            x
        );
    };

    std::vector<glm::vec3> displacedPositions = originalPositions;

    for (size_t i = 0; i < originalPositions.size(); i++)
    {
        glm::vec3 currentPos = originalPositions[i];
        glm::vec3 totalDisplacement(0.0f);

        for (const auto& ball : balls)
        {
            glm::vec3 direction = ball->getPosition() - currentPos;
            float distance = glm::length(direction);

            float softening = 2.0f * ball->getRadius();
            if (distance > 0.001f)
            {
                float influence = (ball->getMass() * warpStrength) / (distance + softening);
                totalDisplacement += glm::normalize(direction) * influence;
            }
        }

        displacedPositions[i] += totalDisplacement;
    }

    int smoothingPasses = 5;
    std::vector<glm::vec3> smoothedPositions = displacedPositions;
    for (int pass = 0; pass < smoothingPasses; pass++)
    {
        for (int z = 1; z < gridDepth; z++)
        {
            for (int y = 1; y < gridHeight; y++)
            {
                for (int x = 1; x < gridWidth; x++)
                {
                    glm::vec3 pX1 = displacedPositions[get_index(x+1, y, z)];
                    glm::vec3 pX0 = displacedPositions[get_index(x-1, y, z)];
                    glm::vec3 pY1 = displacedPositions[get_index(x, y+1, z)];
                    glm::vec3 pY0 = displacedPositions[get_index(x, y-1, z)];
                    glm::vec3 pZ1 = displacedPositions[get_index(x, y, z+1)];
                    glm::vec3 pZ0 = displacedPositions[get_index(x, y, z-1)];

                    glm::vec3 averagePosition = (pX1 + pX0 + pY1 + pY0 + pZ1 + pZ0) / 6.0f;

                    size_t currentIndex = get_index(x, y, z);
                    smoothedPositions[currentIndex] = glm::mix(displacedPositions[currentIndex], averagePosition, 0.5f);
                }
            }
        }
        displacedPositions = smoothedPositions;
    }

    for (size_t i = 0; i < displacedPositions.size(); i++)
    {
        vertices[i * 3 + 0] = displacedPositions[i].x;
        vertices[i * 3 + 1] = displacedPositions[i].y;
        vertices[i * 3 + 2] = displacedPositions[i].z;
    }

    updateVertexBuffer();
}

void Grid::draw(Shader &shaderProgram) const
{
    shaderProgram.use();

    glm::mat4 model = glm::mat4(1.0f);
    shaderProgram.setMat4("model", model);
    shaderProgram.setVec4("color", glm::vec4(0.4f, 0.7f, 1.0f, 0.35f));

    glLineWidth(1.0f);
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Grid::createGrid()
{
    vertices.clear();
    indices.clear();
    originalPositions.clear();

    // create vertices
    for (int z = 0; z <= gridDepth; z++)
    {
        for (int y = 0; y <= gridHeight; y++)
        {
            for (int x = 0; x <= gridWidth; x++)
            {
                float xPos = x * cellSize;
                float yPos = y * cellSize;
                float zPos = z * cellSize;

                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(zPos);

                originalPositions.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }
    }

    auto idx = [&](int x, int y, int z){
        return static_cast<unsigned int>(
            z * (gridWidth + 1) * (gridHeight + 1) + 
            y * (gridWidth + 1) + 
            x
        );
    };

    // X axis lines
    for (int z = 0; z <= gridDepth; z++)
    {
        for (int y = 0; y <= gridHeight; y++)
        {
            for (int x = 0; x < gridWidth; x++)
            {
                indices.push_back(idx(x, y, z));
                indices.push_back(idx(x + 1, y, z));
            }
        }
    }

    // Y axis lines
    for (int z = 0; z <= gridDepth; z++)
    {
        for (int x = 0; x <= gridWidth; x++)
        {
            for (int y = 0; y < gridHeight; y++)
            {
                indices.push_back(idx(x, y, z));
                indices.push_back(idx(x, y + 1, z));
            }
        }
    }

    for (int y = 0; y <= gridHeight; y++)
    {
        for (int x = 0; x <= gridWidth; x++)
        {
            for (int z = 0; z < gridDepth; z++)
            {
                indices.push_back(idx(x, y, z));
                indices.push_back(idx(x, y, z + 1));
            }
        }
    }

}

void Grid::updateVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
}
