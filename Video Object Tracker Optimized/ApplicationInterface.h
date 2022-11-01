#pragma once
#include "SFML/Graphics.hpp"

float get_fit_sprite_scaling(
    float WINDOW_HEIGHT, float WINDOW_WIDTH,
    float sprite_width, float sprite_height,
    float width_margin_min, float height_margin_min)
{
    float scaling_ratio_height = (float)WINDOW_HEIGHT / (sprite_height * height_margin_min);
    float scaling_ratio_width = (float)WINDOW_WIDTH / (sprite_width * width_margin_min);

    float min_scaling = std::min(scaling_ratio_height, scaling_ratio_width);

    return min_scaling;
}



void draw_rectangle_border(sf::RenderWindow& wind, float scaling_factor, size_t img_center_x,
    size_t img_center_y, size_t width_px, size_t height_px, size_t thickness, sf::Color col)
{
    sf::RectangleShape rect;

    img_center_x = img_center_x - width_px / 2 + thickness;
    img_center_y = img_center_y - height_px / 2 + thickness;

    rect.setPosition(img_center_x * scaling_factor, img_center_y * scaling_factor);

    rect.setSize(sf::Vector2f(width_px * scaling_factor, height_px * scaling_factor));
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(col);
    rect.setOutlineThickness(thickness);
    wind.draw(rect);
}
