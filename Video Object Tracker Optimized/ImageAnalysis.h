#pragma once

double difference_metric_at_pos(sf::Image& base_image, sf::Image& compare_template,
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
