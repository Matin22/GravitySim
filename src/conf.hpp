#pragma once

namespace conf
{
    // window configs
    constexpr int SCREEN_WIDTH = 1000;
    constexpr int SCREEN_HEIGHT = 1000;
    constexpr int MAX_FRAMERATE = 144; // fps
    constexpr float dt = 1.0f / static_cast<float>(MAX_FRAMERATE);

    constexpr int MAX_TRAIL_LENGTH = 10 * MAX_FRAMERATE; // in seconds

    // constexpr float G_CONSTANT = 6.67430e-11;
    constexpr float G_CONSTANT = 1000.0f;

    constexpr glm::vec2 OBJECT1_POSITION    = glm::vec2(conf::SCREEN_WIDTH / 2.0f, conf::SCREEN_HEIGHT / 2.0f);
    constexpr float     OBJECT1_RADIUS      = 30.0f;
    constexpr glm::vec2 OBJECT1_VELOCITY    = glm::vec2(0.0f, 0.0f);
    constexpr glm::vec4 OBJECT1_COLOR       = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    
    constexpr glm::vec2 OBJECT2_POSITION    = glm::vec2(conf::SCREEN_WIDTH / 2.0f, conf::SCREEN_HEIGHT / 2.0f + 300.0f);
    constexpr float     OBJECT2_RADIUS      = 10.0f;
    constexpr glm::vec2 OBJECT2_VELOCITY    = glm::vec2(18.2f, 0.0f);
    constexpr glm::vec4 OBJECT2_COLOR       = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
}