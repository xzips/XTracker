#include <SFML/Graphics.hpp>
#include <iostream>
#include "image_pixel_manipulation.h"

constexpr size_t WINDOW_WIDTH = 1920;
constexpr size_t WINDOW_HEIGHT = 1080;


//returns scale ratio used
float scale_sprite_to_params(sf::Sprite &s, sf::Texture &tex, float width_margin_min, float height_margin_min)
{


    float scaling_ratio_height = (float)WINDOW_HEIGHT / (float)tex.getSize().y;
    float scaling_ratio_width = (float)WINDOW_WIDTH / (float)tex.getSize().x * width_margin_min;

    float min_scaling = std::min(scaling_ratio_height, scaling_ratio_width);

    s.setScale(min_scaling, min_scaling);
    return min_scaling;
}


void draw_rectangle_border(sf::RenderWindow& wind, float scaling_factor, float img_center_x, float img_center_y, float width_px, float height_px)
{
    sf::RectangleShape rect;
    rect.setPosition(img_center_x * scaling_factor, img_center_y * scaling_factor);
    rect.setScale(width_px * scaling_factor, height_px * scaling_factor);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(3);
    wind.draw(rect);
}






int main()
{
    sf::Texture example_frame_texture;
    if (!example_frame_texture.loadFromFile("C:/Users/Aspen/Desktop/TrackerX/frame_0.png"))
    {
        std::cout << "frame_0.png failed to load\n";
        exit(-1);
    }

    //add_bounding_rectangle(example_frame_texture, 300, 300, 250, 200, 8);

    sf::Texture cropped_tex;

    float crop_x = 200;
    float crop_y = 200;


    cropped_tex.create(crop_x, crop_y);

   
    sf::Image rect_img = extract_rectangle_as_image(example_frame_texture, 510, 1230, crop_x, crop_y);
    cropped_tex.loadFromImage(rect_img);


    
    

    sf::Sprite frame_sprite;
    float frame_scale = scale_sprite_to_params(frame_sprite, example_frame_texture, 0.8, 1);
    frame_sprite.setTexture(example_frame_texture);





    sf::Sprite extracted_sprite;
    float extract_scale = scale_sprite_to_params(extracted_sprite, cropped_tex, 0.2, 1);
    extracted_sprite.setTexture(cropped_tex);
    extracted_sprite.setPosition(WINDOW_WIDTH - extract_scale*crop_x, 0);


    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    window.close();
               }
            }
        }

        window.clear();
        window.draw(frame_sprite);
        window.draw(extracted_sprite);

       //std::cout << frame_scale << std::endl;
        //draw_rectangle_border(window, frame_scale, 500, 1200, crop_x, crop_y);

        window.display();
    }

    return 0;
}
