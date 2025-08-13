#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "shader.hpp"
#include "conf.hpp"

class ballObject
{
public:
    ballObject(glm::vec2 pos, float radius, glm::vec2 velocity = glm::vec2(0.0f, 0.0f), glm::vec4 color = glm::vec4(1, 0, 0, 1));
    ~ballObject();

    ballObject(const ballObject&) = delete;

    void update(float dt);
    void draw(Shader shaderProgram) const;
    void applyForce(glm::vec2& force);
    void applyGravity(const ballObject *other);

    glm::vec2 position;
    glm::vec2 velocity;
    float radius;
    float mass;
    glm::vec4 color;
    glm::vec2 acceleration;

    std::vector<float> trailVertices;
    int maxTrailLength = conf::MAX_TRAIL_LENGTH;
    void drawTrail(Shader shaderprogram) const;

private:
    GLuint VAO, VBO;
    GLuint trailVAO, trailVBO;
    
    std::vector<float> vertices;

    int frameCounter = 0;

    void createVertices(int segments = 32);
};