#pragma once

namespace conf
{
    // window configs
    constexpr int SCREEN_WIDTH = 800;
    constexpr int SCREEN_HEIGHT = 600;
    constexpr int MAX_FRAMERATE = 144;
    constexpr float dt = 1.0f / static_cast<float>(MAX_FRAMERATE);

    // constexpr float G_CONSTANT = 6.67430e-11;
    constexpr float G_CONSTANT = 1000.0f;

}