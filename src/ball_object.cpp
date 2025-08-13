#include <math.h>

#include "ball_object.hpp"
#include "conf.hpp"

ballObject::ballObject(glm::vec2 pos, float radius, glm::vec2 velocity, glm::vec4 color)
    : position(pos), radius(radius), velocity(velocity), color(color), mass(1.0f), acceleration(0.0f, 0.0f)
{
    createVertices();
    
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // stride and offset for position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);


    // trail buffers
    glGenBuffers(1, &trailVBO);
    glGenVertexArrays(1, &trailVAO);
    glBindVertexArray(trailVAO);
    glBindBuffer(GL_ARRAY_BUFFER, trailVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

ballObject::~ballObject()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &trailVAO);
    glDeleteBuffers(1, &trailVBO);
}

void ballObject::update(float dt)
{
    trailVertices.push_back(position.x);
    trailVertices.push_back(position.y);
    trailVertices.push_back(0.0f);

    if (trailVertices.size() > (maxTrailLength * 3) && maxTrailLength != 0)
        trailVertices.erase(trailVertices.begin(), trailVertices.begin() + 3);

    velocity += acceleration * dt;
    position += velocity * dt;
    acceleration = glm::vec2(0.0f, 0.0f);
}

void ballObject::draw(Shader shaderProgram) const
{
    shaderProgram.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(radius, radius, 1.0f));
 
    shaderProgram.setMat4("model", model);
    shaderProgram.setVec4("color", color);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 3);
    glBindVertexArray(0);
}

void ballObject::applyForce(glm::vec2 &force)
{
    acceleration += force / mass;
}

void ballObject::applyGravity(const ballObject *other)
{
    glm::vec2 direction = other->position - this->position;
    float distance = glm::length(direction);

    if (distance > 0)
    {
        direction = glm::normalize(direction);
        float force = (conf::G_CONSTANT * other->mass * this->mass) / (distance * distance);
        glm::vec2 forceVector = direction * force;
        applyForce(forceVector);
    }
}

void ballObject::drawTrail(Shader shaderprogram) const
{
    if (trailVertices.size() < 2 * 3)
        return;

    glBindVertexArray(trailVAO);
    glBindBuffer(GL_ARRAY_BUFFER, trailVBO);
    glBufferData(GL_ARRAY_BUFFER, trailVertices.size() * sizeof(float), trailVertices.data(), GL_DYNAMIC_DRAW);
    shaderprogram.use();
    glm::mat4 model = glm::mat4(1.0f);
    shaderprogram.setMat4("model", model);

    glm::vec4 trailColor = color;
    trailColor.a = 0.5f;
    shaderprogram.setVec4("color", trailColor);

    glDrawArrays(GL_LINE_STRIP, 0, trailVertices.size() / 3);
    glBindVertexArray(0);
}

void ballObject::createVertices(int segments)
{
    vertices.clear();

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.14159265359f * i / segments;
        float x = cos(angle);
        float y = sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }
}