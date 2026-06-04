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
#include "grid3D.hpp"
#include "crosshair.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::vector<ballObject *> balls;

// framerate

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool gridEnabled = true;

Camera camera(glm::vec3(conf::SCREEN_X / 2.0f, 0.0f, conf::SCREEN_Z));

bool firstMouse = true;
float lastX = conf::SCREEN_X / 2.0f;
float lastY = conf::SCREEN_Y / 2.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow *window, double offsetX, double offSetY)
{
    (void)window;
    (void)offsetX;
    camera.ProcessMouseScroll(offSetY);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;
    
    if (key == GLFW_KEY_G && action == GLFW_PRESS) gridEnabled = !gridEnabled;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; // coordinates reversed
    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
    (void)mode;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        glm::vec3 normalizedDirection = glm::normalize(camera.getDirection());

        glm::vec3 spawnPos = camera.getPosition() + normalizedDirection * 50.f;

        glm::vec3 velocity = normalizedDirection * conf::SHOT_SPEED;

        // Create new ball at mouse position
        ballObject *newBall = new ballObject(
            spawnPos,
            conf::SHOT_RADIUS,
            velocity,
            conf::SHOT_MASS,
            glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
        );

        balls.push_back(newBall);
        }
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FRONT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACK, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.ProcessKeyboard(DOWN, deltaTime);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(conf::SCREEN_X, conf::SCREEN_Y, "GravitySim", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    // import and create shader
    Shader myShader("res\\shaders\\shader.vert", "res\\shaders\\shader.frag");
    Grid gravityGrid(100.0f);
    Crosshair crosshair(conf::CROSSHAIR_SIZE);

    myShader.use();
    
    balls =
    {
        new ballObject(conf::OBJECT1_POSITION, conf::OBJECT1_RADIUS, conf::OBJECT1_VELOCITY, conf::OBJECT1_MASS, conf::OBJECT1_COLOR),
        new ballObject(conf::OBJECT2_POSITION, conf::OBJECT2_RADIUS, conf::OBJECT2_VELOCITY, conf::OBJECT2_MASS, conf::OBJECT2_COLOR)
    };

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // user input handling
        processInput(window);

        for (size_t i = 0; i < balls.size(); ++i)
        {
            for (size_t j = i + 1; j < balls.size(); ++j)
            {
                balls[i]->applyGravity(balls[j]);
                balls[j]->applyGravity(balls[i]);
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
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        // glm::vec3 camPos(conf::SCREEN_X * 0.5f, conf::SCREEN_Y * 0.5f, 1200.0f);
        // glm::vec3 target(conf::SCREEN_X * 0.5f, conf::SCREEN_Y * 0.5f, 0.0f);
        // glm::mat4 view = glm::lookAt(camPos, target, glm::vec3(0, 1, 0));
        glm::mat4 view = camera.GetViewMatrix();
        
        // set projection matrix for every object
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), static_cast<float>(conf::SCREEN_X) / static_cast<float>(conf::SCREEN_Y), 0.1f, 2000.0f);
        
        myShader.setMat4("view", view);
        myShader.setMat4("projection", projection);

        if (gridEnabled)
        {
            gravityGrid.updateGrid(balls, 100.0f);
            gravityGrid.draw(myShader);
        }
        
        if (conf::TRAIL_ENABLED)
        {
            for (auto ball : balls)
            {
                ball->drawTrail(myShader);
            }
        }
        
        for (auto ball : balls)
        {
            ball->draw(myShader);
        }

        crosshair.draw(myShader);

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