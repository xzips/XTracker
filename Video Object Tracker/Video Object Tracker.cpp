#include <SFML/Graphics.hpp>
#include <iostream>
#include "image_pixel_manipulation.h"

constexpr size_t WINDOW_WIDTH = 1080;
constexpr size_t WINDOW_HEIGHT = 780;


//returns scale ratio used
float scale_sprite_to_params(sf::Sprite &s, sf::Texture &tex, float width_margin_min, float height_margin_min)
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




sf::Uint8 pixel_difference_f(sf::Uint8 p0, sf::Uint8 p1)
{
    return abs((int32_t)p0 - (int32_t)p1);
}

sf::Image difference_image_at_pos(sf::Image &base_image, sf::Image compare_template,
    uint32_t center_x, uint32_t center_y, uint32_t rect_width, uint32_t rect_height)
{
  

    //return compare_template;
    size_t compare_imgs_sz = sizeof(sf::Uint8) * compare_template.getSize().x * compare_template.getSize().y * 4;
    
    //THIS MUST BE FREED LATER!!!!!
    //EXPERIMENT WITH USING IMAGE INSTEAD OF PIXELS???
    sf::Uint8* cropped_base_pixels = extract_rectangle_pixels_from_image(base_image,
        center_x, center_y, rect_width, rect_height);

    sf::Uint8* difference_image_pixels = (sf::Uint8*)malloc(compare_imgs_sz);
    if (difference_image_pixels == nullptr)
    {
        free(cropped_base_pixels);

        std::cout << "mem manage fuckd up contact devlop" << std::endl;
        exit(-1);
    }
    
    
    const sf::Uint8* tplt_pixels_ptr = compare_template.getPixelsPtr();

    //alphas is problem

    size_t pixel_sz_difference = sizeof(sf::Uint8) * compare_template.getSize().x * compare_template.getSize().y;
    for (size_t i = 0; i < compare_imgs_sz; i+=4)
    {
        //int32_t img_a_px = compare_template_pixels_ptr[i];
        //int32_t img_b_px = cropped_base_pixels[i];

        //difference_image_pixels[i] = abs(img_a_px - img_b_px);
        difference_image_pixels[i+0] = pixel_difference_f(tplt_pixels_ptr[i], cropped_base_pixels[i]);
        difference_image_pixels[i+1] = pixel_difference_f(tplt_pixels_ptr[i+1], cropped_base_pixels[i+1]);
        difference_image_pixels[i+2] = pixel_difference_f(tplt_pixels_ptr[i+2], cropped_base_pixels[i+2]);
        difference_image_pixels[i+3] = 255;
       
        
    }


    //std::cout << (int)difference_image_pixels[350] << "\n";
   // std::cout << (int)difference_image_pixels[1000] << "\n";
    //std::cout << (int)difference_image_pixels[159999] << "\n";


    sf::Image difference_image;
    difference_image.create(rect_width, rect_height, difference_image_pixels);

    std::cout << (int)difference_image.getPixelsPtr()[350] << "\n";

    free(cropped_base_pixels);
    free(difference_image_pixels);

    return difference_image;
    
}




int main()
{
    sf::Texture example_frame_texture;
    sf::Image example_frame_image;
    
    //C:/Users/aspen/Desktop/firmware-onboarding/XTracker/frame_0.png
    //C:/Users/Aspen/Desktop/TrackerX/frame_0.png
    std::string filename = "C:/Users/aspen/Desktop/firmware-onboarding/XTracker/frame_0.png";

    if (!example_frame_texture.loadFromFile(filename) || !example_frame_image.loadFromFile(filename))
    {
        std::cout << "frame_0.png failed to load\n";
        exit(-1);
    }



    //add_bounding_rectangle(example_frame_texture, 300, 300, 250, 200, 8);

    

    float crop_x = 200;
    float crop_y = 200;

    uint32_t crop_center_x = 510;
    uint32_t crop_center_y = 1230;


    sf::Texture cropped_tex;
    cropped_tex.create(crop_x, crop_y);
    sf::Image rect_img = extract_rectangle_as_image(example_frame_texture, crop_center_x, crop_center_y, crop_x, crop_y);
    cropped_tex.loadFromImage(rect_img);


    sf::Image difference_image = difference_image_at_pos(example_frame_image, rect_img, 400, 1000, crop_x, crop_y);

    std::cout << (int)difference_image.getPixelsPtr()[350] << "\n";

    sf::Texture difference_tex;
    difference_tex.loadFromImage(difference_image);
    

   // sf::Texture cropped_tex;

   // sf::Texture cropped_tex_2;
    //cropped_tex_2.create(crop_x, crop_y);
    //sf::Image rect_img_2 = extract_rectangle_as_image(example_frame_texture, crop_center_x, crop_center_y, crop_x, crop_y);
    
    //cropped_tex_2.loadFromImage(rect_img_2);


    //std::cout << "test difference: " << images_mean_difference(rect_img, rect_img_2) << "\n";
    
    

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

        window.clear();
        window.draw(frame_sprite);
        window.draw(extracted_sprite);
        window.draw(difference_sprite);

        draw_rectangle_border(window, frame_scale, crop_center_x, crop_center_y, crop_x, crop_y, 3);

        window.display();
    }

    return 0;
}
