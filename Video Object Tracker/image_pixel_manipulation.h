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

        /*
        sf::Uint8 red = pByteBuffer[4 * i];
        sf::Uint8 green = pByteBuffer[4 * i + 1];
        sf::Uint8 blue = pByteBuffer[4 * i + 2];
        sf::Uint8 alpha = pByteBuffer[4 * i + 3];
        */
    }

    

    
    extracted_image.create(rect_width, rect_height, extracted_image_pixels);
    free(extracted_image_pixels);

    return extracted_image;

}


void add_bounding_rectangle(sf::Texture& tex, uint32_t rectangle_origin_x, uint32_t rectangle_origin_y,
    uint32_t rect_width, uint32_t rect_height, uint32_t thickness)
{
    sf::Image image = tex.copyToImage();

    // Optional show case: read size of image (not used in this function)
    int width = image.getSize().x;
    int height = image.getSize().y;

    // Get color of specific position
    //sf::Color imagecolor = image.getPixel(x, y);




    // Update texture


    tex.loadFromImage(image);


    sf::Color color = sf::Color::Magenta;
    //top bar
    for (int t = 0; t < thickness; t++)
    {
        for (int x = 0; x < rect_width; x++)
        {
            image.setPixel(x + rectangle_origin_x, rectangle_origin_y + t, color);
        }

    }

    //lower bar
    for (int t = 0; t < thickness; t++)
    {
        for (int x = 0; x < rect_width; x++)
        {
            image.setPixel(x + rectangle_origin_x, rectangle_origin_y + t + rect_height, color);
        }

    }
    //left bar
    for (int t = 0; t < thickness; t++)
    {
        for (int y = 0; y < rect_height; y++)
        {
            image.setPixel(rectangle_origin_x + t, y + rectangle_origin_y, color);
        }

    }
    //right bar
    for (int t = 0; t < thickness; t++)
    {
        for (int y = 0; y < rect_height; y++)
        {
            image.setPixel(rectangle_origin_x + t + rect_width, y + rectangle_origin_y, color);
        }

    }
    tex.loadFromImage(image);






}