#pragma once
#include <SFML/Graphics.hpp>



sf::Image extract_rectangle_as_image(const sf::Texture& tex, size_t center_x, size_t center_y, size_t rect_width, size_t rect_height)
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


//NO ERROR CHECKING, INPUTS BETTER BE VALID!!! :(
double difference_metric_at_pos(const sf::Uint8 *base_pixels_ptr, const sf::Uint8 *template_pixels_ptr,
    const uint32_t center_x, const uint32_t center_y, const uint32_t rect_width, const uint32_t rect_height,
    const uint32_t base_width, const uint32_t base_height)
{
    //for each row, find out where the fuck we are and compare pixels 
    for (size_t rect_row = 0; rect_row < rect_height; ++rect_row)
    {
        for (size_t rect_col = 0; rect_col < rect_width; rect_col++)
        {
            const size_t base_row_indx = (rect_row + center_y) - (rect_width >> 1);
            
            const size_t base_pixels_px_index = (rect_col * 4) + base_row_indx * base_width * 4;

            const size_t template_pixels_px_index = (rect_col * 4) + rect_row * rect_width * 4;;

























                



        }
     
    }











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
        sum_px_diff += square_pixel_difference_f(tplt_pixels_ptr[i], cropped_base_pixels[i]);
        //sum_px_diff += abs(tplt_pixels_ptr[i] - cropped_base_pixels[i]);
        sum_px_diff += square_pixel_difference_f(tplt_pixels_ptr[i + 1], cropped_base_pixels[i + 1]);
        // sum_px_diff += abs(tplt_pixels_ptr[i+1] - cropped_base_pixels[i+1]);
        sum_px_diff += square_pixel_difference_f(tplt_pixels_ptr[i + 2], cropped_base_pixels[i + 2]);
        // sum_px_diff += abs(tplt_pixels_ptr[i+2] - cropped_base_pixels[i+2]);
    }

    free(cropped_base_pixels);


    return (double)sum_px_diff / (double)compare_imgs_sz;

}
