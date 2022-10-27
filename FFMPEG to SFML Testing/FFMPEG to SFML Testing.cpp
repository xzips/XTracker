#include <iostream>
#include <SFML/Graphics.hpp>



//ffmpeg -i earth_video.mp4 -vf select='between(n\,22,29)' output%d.bmp 
void ReadBMP_To_RGBA_Ptr(unsigned char * data_ptr, const char* filename, size_t offset)
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
    //size_t total_resultant_bytes = height * width * 4;
    //size_t resultant_byte_counter = height*width * 4 - 4;
 
    for (uint32_t row = 0; row < height; row+=1)
    {
        int bytes_read = fread(row_pxs, 1, padded_width * 3, f);
        //std::cout << "Bytes Read: " << bytes_read << std::endl;
        for (uint32_t col_indx = 0; col_indx < width; col_indx++)
        {
           // std::cout << "Current R Value is: " << (uint32_t)(uint8_t)row_pxs[col_indx * 3] << std::endl;
            size_t transformed_img_c_pix_ptr = 4 * (row * width + col_indx) + offset;
            data_ptr[transformed_img_c_pix_ptr + 0] = row_pxs[col_indx * 3 + 2]; //BGR not RGB
            data_ptr[transformed_img_c_pix_ptr + 1] = row_pxs[col_indx * 3 + 1];
            data_ptr[transformed_img_c_pix_ptr + 2] = row_pxs[col_indx * 3 + 0];
            data_ptr[transformed_img_c_pix_ptr + 3] = 255;
        }

    }
    fclose(f);

    free(row_pxs);
}


sf::Vector2i GetBmpDims(const char* frame_0_path)
{
    FILE* f = fopen(frame_0_path, "rb");

    if (f == NULL) throw "Argument Exception";
    
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    fclose(f);

    return sf::Vector2i(width, height);
  
}






int main(int argc, char** argv)
{
    const char bmp_path[] = "C:/Users/Aspen/Desktop/XTracker/bmp_sample2.bmp";

    sf::Vector2i img_dims = GetBmpDims(bmp_path);

    size_t img_pix_count = img_dims.x * img_dims.y;

    unsigned char* loaded_bmp_rgba = (unsigned char*)malloc(img_pix_count * 4);

    memset(loaded_bmp_rgba, 0, img_pix_count * 4);

    ReadBMP_To_RGBA_Ptr(loaded_bmp_rgba, bmp_path, 0);

    for (uint32_t i = 0; i < img_pix_count; i++)
    {
        std::cout << "RGBA for PX " << i << ": ("
            << (uint32_t)loaded_bmp_rgba[i * 4 + 0] << ", "
            << (uint32_t)loaded_bmp_rgba[i * 4 + 1] << ", "
            << (uint32_t)loaded_bmp_rgba[i * 4 + 2] << ", "
            << (uint32_t)loaded_bmp_rgba[i * 4 + 3] << ")" << std::endl;
    }

    


    sf::Uint8 *pixels =

    

    return 0;

}