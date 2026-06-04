#include "crosshair.hpp"

Crosshair::Crosshair(float size) : size(size)
{
    float vertices[] = {
        -size, 0.0f, 0.0f,
        size, 0.0f, 0.0f,
        0.0f, -size, 0.0f,
        0.0f, size, 0.0f};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

Crosshair::~Crosshair()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Crosshair::draw(Shader &shader)
{
    glm::mat4 ortho = glm::ortho(0.0f, (float)conf::SCREEN_X, 0.0f, (float)conf::SCREEN_Y, -1.0f, 1.0f);
    shader.setMat4("projection", ortho);
    shader.setMat4("view", glm::mat4(1.0f));
    shader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(conf::SCREEN_X / 2.0f, conf::SCREEN_Y / 2.0f, 0.0f)));

    glDisable(GL_DEPTH_TEST);
    shader.setVec4("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    glLineWidth(2.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 4);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(1.0f);
}