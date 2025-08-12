#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>

#include <iostream>

#include <vector>

#include "shader.hpp"
#include "camera.hpp"
#include "ball_object.hpp"
#include "conf.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string parseShader(const std::string &filePath)
{
    std::ifstream file(filePath);
    std::stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow *window, double offsetX, double offSetY)
{
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(conf::SCREEN_WIDTH, conf::SCREEN_HEIGHT, "GravitySim", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetScrollCallback(window, scroll_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // import and create shader
    Shader myShader("res\\shaders\\vshader.glsl", "res\\shaders\\fshader.glsl");

    
    myShader.use();
    
    std::vector<ballObject*> balls =
    {
        new ballObject(glm::vec2(400.0f, 300.0f), 50.0f, glm::vec2(0.0f, 0.0f)),
        new ballObject(glm::vec2(700.0f, 300.0f), 10.0f, glm::vec2(10.0f, -70.0f))
    };
    
    balls[0]->mass = 1000.0f;
    balls[1]->mass = 10.0f;

    while (!glfwWindowShouldClose(window))
    {
        // user input handling
        processInput(window);

        for (auto ball1 : balls)
        {
            for (auto ball2 : balls)
            {
                ball1->applyGravity(ball2);
                ball2->applyGravity(ball1);
            }
        }

        for (auto ball : balls)
        {
            ball->update(conf::dt);
        }

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // set projection matrix for every object
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::ortho(0.0f, static_cast<float>(conf::SCREEN_WIDTH), 0.0f, static_cast<float>(conf::SCREEN_HEIGHT));
        myShader.setMat4("projection", projection);

        for (auto ball : balls)
        {
            ball->draw(myShader);
        }

        // check events and call events and swap buffer frame
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (auto ball : balls)
    {
        delete ball;
    }

    glfwTerminate();
    return 0;
}