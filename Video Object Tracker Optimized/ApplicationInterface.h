#pragma once
#include "SFML/Graphics.hpp"
//returns scale ratio used

float scale_sprite_to_params(sf::Sprite& s,
    float WINDOW_HEIGHT, float WINDOW_WIDTH,
    float sprite_width, float sprite_height,
    float width_margin_min, float height_margin_min)
{


    float scaling_ratio_height = (float)WINDOW_HEIGHT / sprite_height * height_margin_min;
    float scaling_ratio_width = (float)WINDOW_WIDTH / sprite_width * width_margin_min;

    float min_scaling = std::min(scaling_ratio_height, scaling_ratio_width);

    s.setScale(min_scaling, min_scaling);
    return min_scaling;
}


