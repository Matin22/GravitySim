#pragma once

#include <cmath>
#include <iostream>

namespace conf
{
    // window configs
    constexpr float SCREEN_X = 1000.0f;
    constexpr float SCREEN_Y = 1000.0f;
    constexpr float SCREEN_Z = 1000.0f;
    constexpr int MAX_FRAMERATE = 144; // fps
    constexpr float dt = 1.0f / static_cast<float>(MAX_FRAMERATE);
    // constexpr float dt = 1.f;

    // CAMERA SETTINGS
    constexpr float CAMERA_SPEED    = 200.f;
    constexpr float CAMERA_SENS     = 0.1f;

    // constexpr float G_CONSTANT = 6.67430e-11;
    constexpr float G_CONSTANT = 1000.0f;

    constexpr glm::vec3 OBJECT1_POSITION    = glm::vec3(conf::SCREEN_X / 2.0f, conf::SCREEN_Y / 2.0f, 500.0f);
    constexpr float     OBJECT1_RADIUS      = 30.0f;
    constexpr float     OBJECT1_MASS        = 100.f;
    constexpr glm::vec3 OBJECT1_VELOCITY    = glm::vec3(0.0f, 0.0f, 0.0f);
    constexpr glm::vec4 OBJECT1_COLOR       = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    
    constexpr glm::vec3 OBJECT2_POSITION    = glm::vec3(conf::SCREEN_X / 2.0f, conf::SCREEN_Y / 2.0f + 300.0f, 500.0f);
    constexpr float     OBJECT2_MASS        = 1.f;
    constexpr float     OBJECT2_RADIUS      = 10.0f;
    constexpr glm::vec3 OBJECT2_VELOCITY    = glm::vec3(std::sqrt((G_CONSTANT * OBJECT1_MASS)/300.f), 0.0f, 0.0f);
    // constexpr glm::vec3 OBJECT2_VELOCITY    = glm::vec3(16.4f, 0.0f, 0.0f);
    constexpr glm::vec4 OBJECT2_COLOR       = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    // SHOOTING CONF
    constexpr float SHOT_SPEED  = 10.f;
    constexpr float SHOT_MASS   = 100.f;
    constexpr float SHOT_RADIUS = 10.f;

    // OBJECT TRAILS
    constexpr bool  TRAIL_ENABLED       = true;
    constexpr int   MAX_TRAIL_LENGTH    = 0;

    // CROSSHAIR
    constexpr float CROSSHAIR_SIZE = 10.0f;
}