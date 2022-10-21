#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

void draw_img_in(sf::Image img)
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "XTracker Debug Window");

    sf::Texture img_tex;
    img_tex.loadFromImage(img);



    sf::Sprite img_sprite;
    img_sprite.setScale(800.f / img_tex.getSize().x, 800.f / img_tex.getSize().y);
    img_sprite.setTexture(img_tex);


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
        window.draw(img_sprite);

        window.display();
    }
}

sf::Image extract_rectangle_as_image(sf::Texture &tex, size_t center_x, size_t center_y, size_t rect_width, size_t rect_height)
{
    sf::Image extracted_image;
    sf::Image full_image = tex.copyToImage();
    
    //fix this lol
    center_x -= rect_width >> 1;
    center_y -= rect_height >> 1;

   
    //check this
    size_t extracted_img_buff_sz = sizeof(sf::Uint8) * rect_width * rect_height * 4;
    sf::Uint8* extracted_image_pixels = (sf::Uint8*)malloc(extracted_img_buff_sz);
    if (extracted_image_pixels == nullptr)
    {
        std::cout << "somebody(I) fucked up memory management and now we can't have nice things, contact the dev" << std::endl;
    }



    const sf::Uint8* original_image_p_buffer = full_image.getPixelsPtr();

    size_t cur_loc_in_extr_pix = 0;

    size_t numPixels = full_image.getSize().x * full_image.getSize().y * 4;
    //for each row, find out where the fuck we are and memcpy pixels 
    for (size_t row = 0; row < rect_height; ++row)
    {
        size_t row_begin_indx = ((row + center_y) * tex.getSize().x + center_x) * 4;


        sf::Uint8* dst_addr = extracted_image_pixels + cur_loc_in_extr_pix;

        if (dst_addr == nullptr) exit(-1);

        memcpy(dst_addr, original_image_p_buffer + row_begin_indx, 4 * rect_width);

        cur_loc_in_extr_pix += rect_width * 4;
    }
    
    extracted_image.create(rect_width, rect_height, extracted_image_pixels);
    free(extracted_image_pixels);

    return extracted_image;

}

sf::Uint8* extract_rectangle_pixels_from_image(sf::Image& full_image, size_t center_x, size_t center_y, size_t rect_width, size_t rect_height)
{
    sf::Image extracted_image;

    //fix this lol
    center_x -= rect_width >> 1;
    center_y -= rect_height >> 1;


    //check this
    size_t extracted_img_buff_sz = sizeof(sf::Uint8) * rect_width * rect_height * 4;
    sf::Uint8* extracted_image_pixels = (sf::Uint8*)malloc(extracted_img_buff_sz);
    if (extracted_image_pixels == nullptr)
    {
        std::cout << "somebody(I) fucked up memory management and now we can't have nice things, contact the dev" << std::endl;
    }

    const sf::Uint8* original_image_p_buffer = full_image.getPixelsPtr();

    size_t cur_loc_in_extr_pix = 0;

    size_t numPixels = full_image.getSize().x * full_image.getSize().y * 4;
    //for each row, find out where the fuck we are and memcpy pixels 
    for (size_t row = 0; row < rect_height; ++row)
    {
        size_t row_begin_indx = ((row + center_y) * full_image.getSize().x + center_x) * 4;


        sf::Uint8* dst_addr = extracted_image_pixels + cur_loc_in_extr_pix;

        if (dst_addr == nullptr) exit(-1);

        memcpy(dst_addr, original_image_p_buffer + row_begin_indx, 4 * rect_width);

        cur_loc_in_extr_pix += rect_width * 4;
    }

    //REMEMBER TO FREE THIS SHIT CUHHH
    return extracted_image_pixels;

}

double images_mean_difference(sf::Image img_a, sf::Image img_b)
{
    if (img_a.getSize() != img_b.getSize())
    {
        std::cout << "can't get similarity of different sized images, you silly goose, contact dev" << std::endl;
        exit(-1);
    }

    //maybe do this for debugging
    size_t extracted_img_buff_sz = sizeof(sf::Uint8) * img_a.getSize().x * img_a.getSize().y * 4;
    //sf::Uint8* extracted_image_pixels = (sf::Uint8*)malloc(extracted_img_buff_sz);
    //std::cout << "img_buff_sz: " << extracted_img_buff_sz << "\n";

    const sf::Uint8* img_a_px_ptr = img_a.getPixelsPtr();
    const sf::Uint8* img_b_px_ptr = img_b.getPixelsPtr();

    uint64_t absolute_difference_sum = 0;
    for (size_t i = 0; i<extracted_img_buff_sz; i++)
    {
        int32_t img_a_px = img_a_px_ptr[i];
        int32_t img_b_px = img_b_px_ptr[i];

        //mean squared erorr
        absolute_difference_sum += abs(img_a_px - img_b_px);
    }

    double avg_diff = (double)absolute_difference_sum / (double)extracted_img_buff_sz;
    return avg_diff;
}


sf::Uint8 abs_pixel_difference_raw(sf::Uint8 p0, sf::Uint8 p1)
{
    sf::Uint8 diff = std::max(255 - abs((int32_t)p0 - (int32_t)p1), 127);
    return diff;
}

sf::Image difference_image_at_pos(sf::Image& base_image, sf::Image compare_template,
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
    for (size_t i = 0; i < compare_imgs_sz; i += 4)
    {
        difference_image_pixels[i + 0] = abs_pixel_difference_raw(tplt_pixels_ptr[i], cropped_base_pixels[i]);
        difference_image_pixels[i + 1] = abs_pixel_difference_raw(tplt_pixels_ptr[i + 1], cropped_base_pixels[i + 1]);
        difference_image_pixels[i + 2] = abs_pixel_difference_raw(tplt_pixels_ptr[i + 2], cropped_base_pixels[i + 2]);
        difference_image_pixels[i + 3] = 255;
    }


    sf::Image difference_image;
    difference_image.create(rect_width, rect_height, difference_image_pixels);

    //std::cout << (int)difference_image.getPixelsPtr()[350] << "\n";

    free(cropped_base_pixels);
    free(difference_image_pixels);

    return difference_image;

}



double difference_metric_at_pos(sf::Image& base_image, sf::Image &compare_template,
    uint32_t center_x, uint32_t center_y, uint32_t rect_width, uint32_t rect_height)
{
    //return compare_template;
    size_t compare_imgs_sz = sizeof(sf::Uint8) * compare_template.getSize().x * compare_template.getSize().y * 4;

    //THIS MUST BE FREED LATER!!!!!
    //EXPERIMENT WITH USING IMAGE INSTEAD OF PIXELS???
    sf::Uint8* cropped_base_pixels = extract_rectangle_pixels_from_image(base_image,
        center_x, center_y, rect_width, rect_height);

    const sf::Uint8* tplt_pixels_ptr = compare_template.getPixelsPtr();

   

    sf::Image cropped_base_pix_img;
    cropped_base_pix_img.create(rect_width, rect_height, cropped_base_pixels);


    //draw_img_in(compare_template);
   // draw_img_in(cropped_base_pix_img);

    size_t sum_px_diff = 0;
    for (size_t i = 0; i < compare_imgs_sz; i += 4)
    {
        //just rgb and skip alpha
        sum_px_diff += abs(tplt_pixels_ptr[i] - cropped_base_pixels[i]);
        sum_px_diff += abs(tplt_pixels_ptr[i+1] - cropped_base_pixels[i+1]);
        sum_px_diff += abs(tplt_pixels_ptr[i+2] - cropped_base_pixels[i+2]);
    }

    free(cropped_base_pixels);
   

    return (double)sum_px_diff / (double)compare_imgs_sz;

}




//returns the centered position of the best found match
sf::Vector2i search_region_for_match(sf::Image& base_image, sf::Image& target_template,
    uint32_t region_center_x, uint32_t region_center_y, uint32_t region_width, uint32_t region_height)
{
    sf::Vector2i best_match_center_position;
    double smallest_difference = INFINITY;
    sf::Image section_extracted_img;

    for (int i = 0; i < 1; i++)
    {
        size_t compare_x_offset = 0;
        size_t compare_y_offset = 0;

        double cur_diff = difference_metric_at_pos(
            base_image, target_template, region_center_x + compare_x_offset,
            region_center_y + compare_y_offset,
            region_width, region_height);

        if (cur_diff < smallest_difference)
        {
            smallest_difference = cur_diff;

            best_match_center_position = sf::Vector2i(region_center_x + compare_x_offset, region_center_y + compare_y_offset);
        }
    }
    std::cout << "Match Difference: " << smallest_difference << "\n";
    return best_match_center_position;
     
    
    



}
