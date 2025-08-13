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

std::vector<ballObject *> balls;

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        float x = static_cast<float>(xpos);
        float y = static_cast<float>(conf::SCREEN_HEIGHT - ypos);
        
        // Create new ball at mouse position
        ballObject* newBall = new ballObject(
            glm::vec2(x, y), 
            10.0f,  // radius
            glm::vec2(0.0f, 0.0f),  // velocity
            glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)  // blue color
        );
        newBall->mass = 25.0f;
        
        balls.push_back(newBall);
        
        std::cout << "Created new ball at: " << x << ", " << y << std::endl;
    }
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
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glLineWidth(3.0f);

    // import and create shader
    Shader myShader("res\\shaders\\vshader.glsl", "res\\shaders\\fshader.glsl");

    
    myShader.use();
    
    balls =
    {
        // new ballObject(conf::OBJECT1_POSITION, conf::OBJECT1_RADIUS, conf::OBJECT1_VELOCITY, conf::OBJECT1_COLOR),
        // new ballObject(conf::OBJECT2_POSITION, conf::OBJECT2_RADIUS, conf::OBJECT2_VELOCITY, conf::OBJECT2_COLOR)
        new ballObject(glm::vec2(500.0f, 600.0f), 15.0f, glm::vec2(20.0f, 0.0f), conf::OBJECT1_COLOR),
        new ballObject(glm::vec2(500.0f, 400.0f), 15.0f, glm::vec2(-20.0f, 0.0f), conf::OBJECT1_COLOR)
    };
    
    balls[0]->mass = 100.0f;
    balls[1]->mass = 100.0f;

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

        // for (size_t i = 1; i < balls.size(); i++)
        // {
        //     balls[i]->applyGravity(balls[0]);
        // }

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
            ball->drawTrail(myShader);
        }

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