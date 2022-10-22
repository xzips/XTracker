#include <SFML/Graphics.hpp>
#include <iostream>
#include "image_pixel_manipulation.h"
#include <vector>
constexpr size_t WINDOW_WIDTH = 1200;
constexpr size_t WINDOW_HEIGHT = 700;


//returns scale ratio used
float scale_sprite_to_params(sf::Sprite& s, sf::Texture& tex, float width_margin_min, float height_margin_min)
{


    float scaling_ratio_height = (float)WINDOW_HEIGHT / (float)tex.getSize().y;
    float scaling_ratio_width = (float)WINDOW_WIDTH / (float)tex.getSize().x * width_margin_min;

    float min_scaling = std::min(scaling_ratio_height, scaling_ratio_width);

    s.setScale(min_scaling, min_scaling);
    return min_scaling;
}


/*
sf::Vector2i track_template_into_frame(sf::Image& track_template, sf::Image& current_frame, sf::Vector2i last_track_location, sf::Vector2i last_track_)
{


    sf::Vector2i best_position = search_region_for_match(example_frame_image, rect_img, crop_center_x, crop_center_y, crop_x, crop_y);
    std::cout << "Best X: " << best_position.x << " ,Best Y: " << best_position.y << "\n";


}

*/



//ugly globals :(
std::vector <sf::Texture> frames_tex_array;
std::vector <sf::Image>   frames_img_array;





//takes a path like desktop/video_frames/frame_
//DO NOT GIVE THIS FUNCTION SOMETHING LIKE: desktop/video_frames/frame_0.png
void load_png_sequence_to_frames(std::string pathname_without_number_or_extension, size_t n_frames)
{

    for (size_t i = 0; i < n_frames; i++)
    {
        frames_img_array.push_back(sf::Image());
        frames_tex_array.push_back(sf::Texture());


        std::string current_image_name = pathname_without_number_or_extension + std::to_string(i) + ".png";
        if (!frames_img_array[frames_img_array.size() - 1].loadFromFile(current_image_name)
            || !frames_tex_array[frames_img_array.size() - 1].loadFromFile(current_image_name))
        {
            std::cout << current_image_name << " failed to load\n";
            exit(-1);
        }
    }
}




int main()

{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "XTracker");

    std::string base_filename = "C:/Users/aspen/Desktop/firmware-onboarding/XTracker/PendulumFrames/pendulum_frame_";
    
    size_t total_n_frames = 10;

    load_png_sequence_to_frames(base_filename, total_n_frames);
    

    //sf::Texture CurrentFrameTexture;
    sf::Sprite  CurrentFrameSprite;

    //draw_img_in(frames_img_array[0]);

    CurrentFrameSprite.setTexture(frames_tex_array[0]);

    float CurrentFrameScale = scale_sprite_to_params(CurrentFrameSprite, frames_tex_array[0], 0.8, 1);

   
    /*


    sf::Sprite frame_sprite;
    frame_sprite.setTexture(frame_1_texture);
    float frame_scale = scale_sprite_to_params(frame_sprite, frame_1_texture, 0.8, 1);

    uint32_t crop_center_x = 215;
    uint32_t crop_center_y = 250;
    uint32_t crop_x = 30;
    uint32_t crop_y = 30;


    uint32_t search_center_x = 210;
    uint32_t search_center_y = 250;
    uint32_t search_width = 250;
    uint32_t search_height = 150;
   

    sf::Image rect_img = extract_rectangle_as_image(frame_0_texture, crop_center_x, crop_center_y, crop_x, crop_y);

    sf::Texture cropped_tex;
    cropped_tex.create(crop_x, crop_y);
    cropped_tex.loadFromImage(rect_img);



    sf::Sprite extracted_sprite;
    float extract_scale = scale_sprite_to_params(extracted_sprite, cropped_tex, 0.2, 1);
    extracted_sprite.setTexture(cropped_tex);
    extracted_sprite.setPosition(WINDOW_WIDTH - extract_scale * crop_x, 0);


    sf::Vector2i best_position = search_region_for_match(
        frame_1_image, rect_img, crop_center_x,
        crop_center_y, search_width, search_height);

    std::cout << "Best X: " << best_position.x << " ,Best Y: " << best_position.y << "\n";

    */

    bool right_pressed = false;
    bool left_clicked = false;
   // bool right_pressed = false;
    size_t current_frame_idx = 0;

    sf::Vector2i search_region_center(215, 250);
    sf::Vector2i search_region_size(40, 40);

    sf::Vector2i template_center(215, 250);
    sf::Vector2i template_size(200, 150);




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


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if (right_pressed == false && current_frame_idx < total_n_frames - 1)
            {
                current_frame_idx++;
                std::cout << "Frame Advanced to: " << current_frame_idx << std::endl;
               
                CurrentFrameSprite.setTexture(frames_tex_array[current_frame_idx]);

            }
            right_pressed = true;
        }
        else right_pressed = false;

        

     

        window.clear();
        window.draw(CurrentFrameSprite);
        //window.draw(extracted_sprite);

        //draw_rectangle_border(window, frame_scale, best_position.x, best_position.y, crop_x, crop_y, 3, sf::Color::Red);
        //draw_rectangle_border(window, frame_scale, search_center_x, search_center_y, search_width, search_height, 3, sf::Color::Blue);

        window.display();
    }


    frames_tex_array.clear();
    frames_img_array.clear();
    return 0;
}
