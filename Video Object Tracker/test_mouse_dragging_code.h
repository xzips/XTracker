#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "image_pixel_manipulation.h"

constexpr size_t WINDOW_WIDTH = 2180;
constexpr size_t WINDOW_HEIGHT = 1300;


//returns scale ratio used
float scale_sprite_to_params(sf::Sprite& s, sf::Texture& tex, float width_margin_min, float height_margin_min)
{


    float scaling_ratio_height = (float)WINDOW_HEIGHT / (float)tex.getSize().y;
    float scaling_ratio_width = (float)WINDOW_WIDTH / (float)tex.getSize().x * width_margin_min;

    float min_scaling = std::min(scaling_ratio_height, scaling_ratio_width);

    s.setScale(min_scaling, min_scaling);
    return min_scaling;
}


void draw_rectangle_border(sf::RenderWindow& wind, float scaling_factor, size_t img_center_x,
    size_t img_center_y, size_t width_px, size_t height_px, size_t thickness)
{
    sf::RectangleShape rect;


    //fix this

    img_center_x = img_center_x - width_px / 2 + thickness;
    img_center_y = img_center_y - height_px / 2 + thickness;

    rect.setPosition(img_center_x * scaling_factor, img_center_y * scaling_factor);

    rect.setSize(sf::Vector2f(width_px * scaling_factor, height_px * scaling_factor));
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(thickness);
    wind.draw(rect);
}


double similarity_template_at_pos(sf::Image base_image, sf::Image compare_template)
{
    return 0;
    //probably should write this lolz  
}







int main()
{
    sf::Texture example_frame_texture;
    sf::Image example_frame_image;

    //C:/Users/aspen/Desktop/firmware-onboarding/XTracker/frame_0.png
    //C:/Users/Aspen/Desktop/TrackerX/frame_0.png
    //std::string filename = "C:/Users/aspen/Desktop/firmware-onboarding/XTracker/frame_0.png";
    std::string filename = "C:\\Users\\Aspen\\Desktop\\XTracker\\frame_0.png";

    if (!example_frame_texture.loadFromFile(filename) || !example_frame_image.loadFromFile(filename))
    {
        std::cout << "frame_0.png failed to load\n";
        exit(-1);
    }



    //add_bounding_rectangle(example_frame_texture, 300, 300, 250, 200, 8);


    /*
    float crop_x = 100;
    float crop_y = 100;

    uint32_t crop_center_x = 510;
    uint32_t crop_center_y = 1230;


    sf::Texture cropped_tex;
    cropped_tex.create(crop_x, crop_y);
    sf::Image rect_img = extract_rectangle_as_image(example_frame_texture, crop_center_x, crop_center_y, crop_x, crop_y);
    cropped_tex.loadFromImage(rect_img);


    sf::Image difference_image = difference_image_at_pos(example_frame_image, rect_img, 500, 1200, crop_x, crop_y);

    std::cout << (int)difference_image.getPixelsPtr()[350] << "\n";

    sf::Texture difference_tex;
    difference_tex.loadFromImage(difference_image);


    sf::Sprite frame_sprite;
    float frame_scale = scale_sprite_to_params(frame_sprite, example_frame_texture, 0.8, 1);

    frame_sprite.setTexture(example_frame_texture);

    sf::Sprite extracted_sprite;
    float extract_scale = scale_sprite_to_params(extracted_sprite, cropped_tex, 0.2, 1);
    extracted_sprite.setTexture(cropped_tex);
    extracted_sprite.setPosition(WINDOW_WIDTH - extract_scale*crop_x, 0);

    sf::Sprite difference_sprite;
    float difference_scale = scale_sprite_to_params(difference_sprite, difference_tex, 0.2, 1);
    std::cout << "diff scale: " << difference_scale << "\n";
    difference_sprite.setTexture(difference_tex);
    difference_sprite.setPosition(WINDOW_WIDTH - difference_scale * crop_x, difference_scale * crop_y);

    */
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "XTracker");










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


        float mouse_x = sf::Mouse::getPosition(window).x;
        float mouse_y = sf::Mouse::getPosition(window).y;



        float crop_x = 100;
        float crop_y = 100;






        sf::Sprite frame_sprite;
        frame_sprite.setTexture(example_frame_texture);

        float frame_scale = scale_sprite_to_params(frame_sprite, example_frame_texture, 0.8, 1);
        float frame_scale_recip = 1.f / frame_scale;

        uint32_t crop_center_x = (mouse_x * frame_scale_recip); //510
        uint32_t crop_center_y = (mouse_y * frame_scale_recip);//1230;

        //std::cout << "frame_scale: " << frame_scale << "\n";

        if (crop_center_x >= example_frame_texture.getSize().x - crop_x || crop_center_x <= crop_x)
        {
            crop_center_x = crop_x;
        }
        if (crop_center_y >= example_frame_texture.getSize().y - crop_y || crop_center_y <= crop_y)
        {
            crop_center_y = crop_y;
        }



        //std::cout << "crop_center_x: " << crop_center_x << "\n";

        sf::Texture cropped_tex;
        cropped_tex.create(crop_x, crop_y);
        sf::Image rect_img = extract_rectangle_as_image(example_frame_texture, crop_center_x, crop_center_y, crop_x, crop_y);
        cropped_tex.loadFromImage(rect_img);


        sf::Image difference_image = difference_image_at_pos(example_frame_image, rect_img, 500, 1200, crop_x, crop_y);

        //std::cout << (int)difference_image.getPixelsPtr()[350] << "\n";

        sf::Texture difference_tex;
        difference_tex.loadFromImage(difference_image);




        sf::Sprite extracted_sprite;
        float extract_scale = scale_sprite_to_params(extracted_sprite, cropped_tex, 0.2, 1);
        extracted_sprite.setTexture(cropped_tex);
        extracted_sprite.setPosition(WINDOW_WIDTH - extract_scale * crop_x, 0);

        sf::Sprite difference_sprite;
        float difference_scale = scale_sprite_to_params(difference_sprite, difference_tex, 0.2, 1);
        // std::cout << "diff scale: " << difference_scale << "\n";
        difference_sprite.setTexture(difference_tex);
        difference_sprite.setPosition(WINDOW_WIDTH - difference_scale * crop_x, difference_scale * crop_y);





        window.clear();
        window.draw(frame_sprite);
        window.draw(extracted_sprite);
        window.draw(difference_sprite);

        draw_rectangle_border(window, frame_scale, crop_center_x, crop_center_y, crop_x, crop_y, 3);

        window.display();
    }

    return 0;
}
