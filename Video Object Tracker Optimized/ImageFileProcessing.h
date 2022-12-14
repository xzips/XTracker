#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <future>

struct uint32_vec2d
{
    uint32_t x;
    uint32_t y;
    uint32_vec2d(uint32_t x, uint32_t y)
    {
        this->x = x;
        this->y = y;
    }
    uint32_vec2d()
    {
        x = 0;
        y = 0;
    }
    uint32_vec2d(const uint32_vec2d &c)
    {
        x = c.x;
        y = c.y;
    }
};

//ffmpeg -i earth_video.mp4 -vf select='between(n\,22,29)' output%d.bmp 
void ReadBMP_To_RGBA_Ptr(unsigned char* data_ptr, const char* filename, const size_t offset)
{
    FILE* f = fopen(filename, "rb");
    if (f == NULL) throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    uint32_t padded_width = width + (width % 2);
    char* row_pxs = (char*)malloc(padded_width * 3);
    if (row_pxs == nullptr) throw "Malloc failed";
    size_t n_res_bytes = height * width * 4 + 1;

    for (int32_t row = height - 1; row >= 0; row--)
    {
        uint32_t bytes_read = fread(row_pxs, 1, padded_width * 3, f);
        for (uint32_t col_indx = 0; col_indx < width; col_indx++)
        {
            size_t transformed_img_c_pix_ptr = (4 * (row * width + col_indx)) + offset;

            //uint64_t hypothetical_index = 
            data_ptr[transformed_img_c_pix_ptr + 0] = row_pxs[col_indx * 3 + 2]; //BGR not RGB
            data_ptr[transformed_img_c_pix_ptr + 1] = row_pxs[col_indx * 3 + 1];
            data_ptr[transformed_img_c_pix_ptr + 2] = row_pxs[col_indx * 3 + 0];
            data_ptr[transformed_img_c_pix_ptr + 3] = 255;
        }
    }
    fclose(f);
    free(row_pxs);
}


uint32_vec2d GetBmpDims(const char* filepath)
{
    FILE* f = fopen(filepath, "rb");
    if (f == NULL) throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    fclose(f);
    return uint32_vec2d(width, height);
}



/*
Example usage:
Consider a folder with contents:
C:/frames/
    ...
    ->img_6.bmp
    ->img_7.bmp
    ->img_8.bmp
    ->img_9.bmp
    ->img_10.bmp
    ->img_11.bmp
    ->img_12.bmp
    ....

In this case:
base_filepath is "C:/frames/img_"
begin_file_index could be 7
n_images could be 4
img_7.bmp, img_8.bmp, img_9.bmp, img_10.bmp will be loaded into memory
*/
void LoadBmpSequenceToMemory(unsigned char *pixels_memory_ptr, const std::string base_filepath,
    const uint32_t begin_file_index, const uint32_t n_images)
{

    const auto img_dims = GetBmpDims((base_filepath + std::to_string(begin_file_index) + ".bmp").c_str());
    const size_t offset_sz = img_dims.x * img_dims.y * 4;

    for (uint32_t load_index = 0; load_index < n_images; load_index++)
    {
        std::string current_image_filepath = base_filepath + std::to_string(load_index + begin_file_index) + ".bmp";
        ReadBMP_To_RGBA_Ptr(pixels_memory_ptr, current_image_filepath.c_str(), offset_sz * load_index);
    }
}



void ffmpeg_async_call_video2bmp(std::string ffmpeg_exe_path, std::string src_path, std::string dst_path, size_t frame_begin, size_t frame_end)
{
    //ffmpeg -i earth_video.mp4 -vf select='between(n\,22,29)' output%d.bmp 
    std::string command = ffmpeg_exe_path + " -i " + src_path +
        " -vf select='between(n\\," + std::to_string(frame_begin) +
        "," + std::to_string(frame_end) + ")' " + dst_path +"frame%d.bmp";

   // std::cout << "command: " << command << std::endl;

    std::async(system, command.c_str());
}