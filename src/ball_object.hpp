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
    glm::vec3 getPosition() const { return position; }
    float getMass() const { return mass; }
    float getRadius() const { return radius; }

    ballObject(glm::vec3 pos, float radius,
        glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f),
        float mass = 50.0f,
        glm::vec4 color = glm::vec4(1, 0, 0, 1));
    ~ballObject();

    ballObject(const ballObject&) = delete;

    void update(float dt);
    void draw(Shader shaderProgram) const;
    void applyForce(glm::vec3& force);
    void applyGravity(const ballObject *other);

    glm::vec3 position;
    float radius;
    glm::vec3 velocity;
    glm::vec4 color;
    float mass;
    glm::vec3 acceleration;

    std::vector<float> trailVertices;
    int maxTrailLength = conf::MAX_TRAIL_LENGTH;
    void drawTrail(Shader shaderprogram) const;

private:
    GLuint VAO, VBO, EBO;
    GLuint trailVAO, trailVBO;
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    int frameCounter = 0;

    void createVertices(int stacks = 16, int slices = 16);
};