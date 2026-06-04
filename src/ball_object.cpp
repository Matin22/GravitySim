#include <math.h>

#include "ball_object.hpp"
#include "conf.hpp"

ballObject::ballObject(glm::vec3 pos, float radius, glm::vec3 velocity, float mass, glm::vec4 color)
    : position(pos), radius(radius), velocity(velocity), color(color), mass(mass), acceleration(0.0f, 0.0f, 0.0f)
{
    createVertices();
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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
    trailVertices.push_back(position.z);

    if (trailVertices.size() > static_cast<size_t>(maxTrailLength * 3) && maxTrailLength != 0)
        trailVertices.erase(trailVertices.begin(), trailVertices.begin() + 3);

    velocity += acceleration * dt;
    position += velocity * dt;
    acceleration = glm::vec3(0.0f);
}

void ballObject::draw(Shader shaderProgram) const
{
    shaderProgram.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(radius));
 
    shaderProgram.setMat4("model", model);
    shaderProgram.setVec4("color", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void ballObject::applyForce(glm::vec3 &force)
{
    acceleration += force / mass;
}

void ballObject::applyGravity(const ballObject *other)
{
    glm::vec3 direction = other->position - this->position;
    float distance = glm::length(direction);

    if (distance > 0)
    {
        direction = glm::normalize(direction);
        float force = (conf::G_CONSTANT * other->mass * this->mass) / (distance * distance);
        glm::vec3 forceVector = direction * force;
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

    glLineWidth(10.0f);
    glDrawArrays(GL_LINE_STRIP, 0, trailVertices.size() / 3);
    glBindVertexArray(0);
}

void ballObject::createVertices(int stacks, int slices)
{
    vertices.clear();
    indices.clear();
    
    // Generate UV sphere vertices
    for (int i = 0; i <= stacks; ++i) {
        float phi = glm::pi<float>() * i / stacks;  // Latitude
        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * glm::pi<float>() * j / slices;  // Longitude
            
            float x = sin(phi) * cos(theta);
            float y = cos(phi);
            float z = sin(phi) * sin(theta);
            
            vertices.push_back(x);  // Unit sphere, scaled by radius in draw
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }
    
    // Generate indices for triangles in the sphere
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;
            
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}