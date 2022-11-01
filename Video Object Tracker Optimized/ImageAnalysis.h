#pragma once
#include <SFML/Graphics.hpp>

struct PixelRegion
{
    uint32_t region_center_x;
    uint32_t region_center_y;
    uint32_t region_width;
    uint32_t region_height;
    uint32_t base_width;
    uint32_t base_height;
    PixelRegion(
        const uint32_t region_center_x, const uint32_t region_center_y,
        const uint32_t region_width, const uint32_t region_height,
        const uint32_t base_width, const uint32_t base_height)
    {
        this->region_center_x = region_center_x;
        this->region_center_y = region_center_y;
        this->region_width = region_width;
        this->region_height = region_height;
        this->base_width = base_width;
        this->base_height = base_height;
    }
    PixelRegion()
    {
        this->region_center_x = 0;
        this->region_center_y = 0;
        this->region_width = 0;
        this->region_height = 0;
        this->base_width = 0;
        this->base_height = 0;
    }
};

void extract_region_pixels(const sf::Uint8* src_pixels, sf::Uint8* dst_pixels, const PixelRegion r)
{
    for (size_t rect_row = 0; rect_row < r.region_height; ++rect_row)
    {
        for (size_t rect_col = 0; rect_col < r.region_width; rect_col++)
        {
            const size_t base_row_indx = rect_row + (r.region_center_y - (r.region_width >> 1));
            const size_t base_col_indx = rect_col + (r.region_center_x - (r.region_height >> 1));

            const size_t base_pixels_px_index = (base_row_indx * r.base_width + base_col_indx) * 4;
            const size_t template_pixels_px_index = (rect_row * r.region_width + rect_col) * 4;;

            dst_pixels[template_pixels_px_index + 0] = src_pixels[base_pixels_px_index + 0];
            dst_pixels[template_pixels_px_index + 1] = src_pixels[base_pixels_px_index + 1];
            dst_pixels[template_pixels_px_index + 2] = src_pixels[base_pixels_px_index + 2];
            dst_pixels[template_pixels_px_index + 3] = src_pixels[base_pixels_px_index + 3];
        }
    }
}



//NO ERROR CHECKING, INPUTS BETTER BE VALID!!! :(
double difference_metric_at_pos(const sf::Uint8* base_pixels_ptr, const sf::Uint8* template_pixels_ptr,
    const uint32_t center_x, const uint32_t center_y, const uint32_t rect_width, const uint32_t rect_height,
    const uint32_t base_width, const uint32_t base_height)
{
    
    uint64_t cumulative_abs_diff = 0;
    //for each row, find out where the fuck we are and compare pixels efficiently
    for (size_t rect_row = 0; rect_row < rect_height; ++rect_row)
    {
        const size_t base_row_indx = rect_row + (center_y - (rect_width >> 1));
        for (size_t rect_col = 0; rect_col < rect_width; rect_col++)
        {
            
            const size_t base_col_indx = rect_col + (center_x - (rect_height >> 1));

            const size_t base_pixels_px_index = (base_row_indx * base_width + base_col_indx) * 4;
            const size_t template_pixels_px_index = (rect_row * rect_width + rect_col) * 4;;

            //size_t base_pixels_px_index = 0;
            //size_t template_pixels_px_index = 0;


            cumulative_abs_diff += abs(base_pixels_ptr[base_pixels_px_index + 0] - template_pixels_ptr[template_pixels_px_index + 0]);
            cumulative_abs_diff += abs(base_pixels_ptr[base_pixels_px_index + 1] - template_pixels_ptr[template_pixels_px_index + 1]);
            cumulative_abs_diff += abs(base_pixels_ptr[base_pixels_px_index + 2] - template_pixels_ptr[template_pixels_px_index + 2]);
            cumulative_abs_diff += abs(base_pixels_ptr[base_pixels_px_index + 3] - template_pixels_ptr[template_pixels_px_index + 3]);
        }
    }
    return (double)cumulative_abs_diff / (double)(rect_width * rect_height * 4);
}




//returns the centered position of the best found match
uint32_vec2d search_region_for_match(
    const sf::Uint8* base_pixels_ptr, const sf::Uint8* template_pixels_ptr,
    const uint32_t region_center_x,   const uint32_t region_center_y,
    const uint32_t region_width,      const uint32_t region_height,
    const uint32_t base_width,        const uint32_t base_height,
    const uint32_t template_width,    const uint32_t template_height)
{
    double smallest_difference = INFINITY;
    uint32_vec2d best_match_center_position;
    
    //for example: 1 checks every single possible location, while 3 checks every 3 locations in a grid
    size_t search_precision = 1;

    //return uint32_vec2d(region_center_x, region_center_y);

    size_t search_begin_x = region_center_x - region_width / 2;
    size_t search_begin_y = region_center_y - region_height / 2;

    size_t call_count = 0;

    for (int x_segment_idx = 0; x_segment_idx < region_width; x_segment_idx += search_precision)
    {
        for (int y_segment_idx = 0; y_segment_idx < region_height; y_segment_idx += search_precision)
        {
            int32_t compare_x_pos = search_begin_x + search_precision * x_segment_idx;
            int32_t compare_y_pos = search_begin_y + search_precision * y_segment_idx;

            double cur_diff = difference_metric_at_pos(
                base_pixels_ptr, template_pixels_ptr,
                compare_x_pos,
                compare_y_pos, 
                template_width,
                template_height,
                base_width,
                base_height);


            
            if (cur_diff < smallest_difference)
            {
                smallest_difference = cur_diff;         
                best_match_center_position = uint32_vec2d(compare_x_pos, compare_y_pos);
            }
            call_count++;
        }
    }
    //std::cout << "DifferenceMetric called: " << call_count << " times" << std::endl;
    return best_match_center_position;

}