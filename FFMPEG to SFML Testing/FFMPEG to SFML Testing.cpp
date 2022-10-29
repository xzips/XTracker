#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>


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
    size_t n_res_bytes = height * width * 4 + 1;
 
    for (int32_t row = height - 1; row >= 0; row--)
    {
        int bytes_read = fread(row_pxs, 1, padded_width * 3, f);
        //std::cout << "Bytes Read: " << bytes_read << std::endl;
        for (uint32_t col_indx = 0; col_indx < width; col_indx++)
        {
            size_t transformed_img_c_pix_ptr = (4 * (row * width + col_indx)) + offset;

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




using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;



void BmpFileDemo() {
    //const char bmp_path[] = "C:/Users/Aspen/Desktop/XTracker/bmp_sample.bmp";
    const char bmp_path[] = "C:/Users/aspen/Desktop/XTracker/ffmpeg/bin/output1.bmp";


    sf::Vector2i img_dims = GetBmpDims(bmp_path);
    size_t img_pix_count = img_dims.x * img_dims.y;

    unsigned char* loaded_bmp_rgba = (unsigned char*)malloc(img_pix_count * 4);

    if (loaded_bmp_rgba == nullptr) throw "Primary Pixel Array Malloc Failed";
    auto t1 = high_resolution_clock::now();
    ReadBMP_To_RGBA_Ptr(loaded_bmp_rgba, bmp_path, 0);


    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() << "ms for 1 frame\n";


    sf::Uint8* bmp_sf_pixels = loaded_bmp_rgba;

    sf::Texture bmp_tex;
    bmp_tex.create(img_dims.x, img_dims.y);
    bmp_tex.update(bmp_sf_pixels);


    sf::Sprite bmp_sprite;
    bmp_sprite.setTexture(bmp_tex);

    bmp_sprite.setScale(0.6, 0.6);


    sf::RenderWindow window(sf::VideoMode(800, 600), "XTracker - Optimized");

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
        window.draw(bmp_sprite);
        window.display();
    }

    free(loaded_bmp_rgba);
}


int main(int argc, char** argv)
{
   // BmpFileDemo();
    //ffmpeg -i earth_video.mp4 -vframes 1 -c:v rawvideo -f image2pipe -
    // 
    //ffmpeg -i earth_video.mp4 -vframes 1 -pix_fmt rgb32 -f image2pipe -

    FILE* fp;
 
    size_t img_size_memory = 1280 * 720 * 4;

    char* pix_ptr = (char*)malloc(img_size_memory);


    std::string full_cmd = "C:/Users/aspen/Desktop/XTracker/ffmpeg/bin/"
         "ffmpeg -i C:/Users/aspen/Desktop/XTracker/ffmpeg/bin/earth_video.mp4 -vframes 1 -pix_fmt rgb24 -f image2pipe -";

    fp = _popen(full_cmd.c_str(), "r");
    //if (fp == 0) exit(-1);


    char pix_buffer[4];


    //char c;
   // while (std::cin >> c)
   // {
     //   std::cout << "Pixel: " << (uint32_t)(uint8_t)c << "\n";
    //}
    
    size_t pix_c = 0;
    while (fgets(pix_buffer, 4, fp))
    {
        pix_ptr[pix_c * 4] = pix_buffer[0];
        pix_ptr[pix_c * 4+1] = pix_buffer[1];
        pix_ptr[pix_c * 4+2] = pix_buffer[2];
        pix_ptr[pix_c * 4+3] = pix_buffer[3];
        std::cout << "Pixel "<< pix_c << ": " << (uint32_t)(uint8_t)pix_buffer[0] << "\n";
        pix_c++;
    }
      


    //_pclose(fp);






    
    sf::Vector2i img_dims = sf::Vector2i(1280, 720);
    size_t img_pix_count = img_dims.x * img_dims.y;

    //unsigned char* loaded_bmp_rgba = (unsigned char*)malloc(img_pix_count * 4);

    //if (loaded_bmp_rgba == nullptr) throw "Primary Pixel Array Malloc Failed";
   // auto t1 = high_resolution_clock::now();
    //ReadBMP_To_RGBA_Ptr(loaded_bmp_rgba, bmp_path, 0);
    /*

    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() << "ms for 1 frame\n";
    */

    sf::Uint8* bmp_sf_pixels = (sf::Uint8*)pix_ptr;

    sf::Texture bmp_tex;
    bmp_tex.create(img_dims.x, img_dims.y);
    bmp_tex.update(bmp_sf_pixels);


    sf::Sprite bmp_sprite;
    bmp_sprite.setTexture(bmp_tex);

    bmp_sprite.setScale(0.6, 0.6);


    sf::RenderWindow window(sf::VideoMode(800, 600), "XTracker - Optimized");

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
        window.draw(bmp_sprite);
        window.display();
    }

   // free(loaded_bmp_rgba);

   

    return 0;
}