#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>



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
    std::cout << "img_buff_sz: " << extracted_img_buff_sz << "\n";

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