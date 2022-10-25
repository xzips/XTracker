#include <SFML/Graphics.hpp>
#include <iostream>
#include "image_pixel_manipulation.h"
#include <vector>
constexpr size_t WINDOW_WIDTH = 1000;
constexpr size_t WINDOW_HEIGHT = 700;


//returns scale ratio used
float scale_sprite_to_params(sf::Sprite& s, float width, float height, float width_margin_min, float height_margin_min)
{


    float scaling_ratio_height = (float)WINDOW_HEIGHT / height * height_margin_min;
    float scaling_ratio_width = (float)WINDOW_WIDTH / width * width_margin_min;

    float min_scaling = std::min(scaling_ratio_height, scaling_ratio_width);

    s.setScale(min_scaling, min_scaling);
    return min_scaling;
}



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

    //std::string base_filename = "C:/Users/aspen/Desktop/firmware-onboarding/XTracker/PendulumFrames/pendulum_frame_";
    std::string base_filename = "C:/Users/Aspen/Desktop/XTracker/PendulumFrames/pendulum_frame_";
    
   
    sf::Vector2i search_region_size(200, 150);
    sf::Vector2i template_center(400, 480); //400, 480
    sf::Vector2i template_size(20, 20); //20, 20

    size_t total_n_frames = 30;

    load_png_sequence_to_frames(base_filename, total_n_frames);
    
    sf::Vector2i search_region_center = template_center;


    //sf::Texture CurrentFrameTexture;
    sf::Sprite  CurrentFrameSprite;
    sf::Sprite  CurrentTemplateSprite;
    sf::Sprite  CurrentFrameTrackSprite;


    sf::Texture CurrentFrameTrackTexture;
    float CurrentFrameTrackScale = scale_sprite_to_params(CurrentFrameTrackSprite,
        template_size.x, template_size.y, 0.2, 1);

    CurrentFrameTrackSprite.setPosition(WINDOW_WIDTH - CurrentFrameTrackScale * template_size.x,
        CurrentFrameTrackScale * template_size.y);



    CurrentFrameTrackTexture.create(template_size.x, template_size.y);

    //draw_img_in(frames_img_array[0]);

    CurrentFrameSprite.setTexture(frames_tex_array[0]);


    float CurrentFrameScale = scale_sprite_to_params(CurrentFrameSprite,
        frames_tex_array[0].getSize().x, frames_tex_array[0].getSize().y, 0.8, 1);




    bool right_pressed = false;
    bool left_clicked = false;
 
    size_t current_frame_idx = 0;

    //make into function maybe
    sf::Image rect_img = extract_rectangle_as_image(frames_tex_array[0], template_center.x, template_center.y,
        template_size.x, template_size.y);


    sf::Texture cropped_tex;
    cropped_tex.create(template_size.x, template_size.y);
    cropped_tex.loadFromImage(rect_img);
    sf::Sprite extracted_sprite;
    
    float extract_scale = scale_sprite_to_params(extracted_sprite,
        cropped_tex.getSize().x, cropped_tex.getSize().y, 0.2, 1);

    extracted_sprite.setTexture(cropped_tex);
    extracted_sprite.setPosition(WINDOW_WIDTH - extract_scale * template_size.x, 0);




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
            //if (right_pressed == false && current_frame_idx < total_n_frames - 1)
            if (true && current_frame_idx < total_n_frames - 1)
            {
                current_frame_idx++;
                std::cout << "Frame Advanced to: " << current_frame_idx << std::endl;
               
                CurrentFrameSprite.setTexture(frames_tex_array[current_frame_idx]);


                template_center = search_region_for_match(
                    frames_img_array[current_frame_idx], rect_img,
                    template_center.x, template_center.y, search_region_size.x, search_region_size.y);

                search_region_center = template_center;





                sf::Image current_frame_extract = extract_rectangle_as_image(
                    frames_tex_array[current_frame_idx], template_center.x, template_center.y,
                    template_size.x, template_size.y);
               
                //draw_img_in(current_frame_extract);

                CurrentFrameTrackTexture.create(template_size.x, template_size.y);
                CurrentFrameTrackTexture.loadFromImage(current_frame_extract);
                CurrentFrameTrackSprite.setTexture(CurrentFrameTrackTexture);

                //set new
                //rect_img = extract_rectangle_as_image(frames_tex_array[current_frame_idx], template_center.x, template_center.y,
                //   template_size.x, template_size.y);

               // cropped_tex.create(template_size.x, template_size.y);
               // cropped_tex.loadFromImage(rect_img);
                //extracted_sprite.setTexture(cropped_tex);
               // extracted_sprite.setPosition(WINDOW_WIDTH - ex  tract_scale * template_size.x, 0);

            }
            right_pressed = true;
        }
        else right_pressed = false;

        

     

        window.clear();
        window.draw(CurrentFrameSprite);
        window.draw(extracted_sprite);
        window.draw(CurrentFrameTrackSprite);

        draw_rectangle_border(window, CurrentFrameScale, template_center.x, template_center.y, template_size.x, template_size.y, 3, sf::Color::Red);
        draw_rectangle_border(window, CurrentFrameScale, search_region_center.x, search_region_center.y, search_region_size.x, search_region_size.y, 3, sf::Color::Blue);

        window.display();
    }


    frames_tex_array.clear();
    frames_img_array.clear();
    return 0;
}
