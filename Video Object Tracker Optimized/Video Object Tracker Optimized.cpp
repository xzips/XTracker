
#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "ImageFileProcessing.h"
#include "ImageDebuggingTools.h"
#include "ImageAnalysis.h"
#include <string>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


/*
Current Goals:
Reimplement forward tracking


*/


void XTrackerMain()
{
    //load images into buffer
    std::string base_path = "C:/Users/Aspen/Desktop/XTracker/BmpFrames/output";
    size_t n_images = 10;
    auto img_dims = GetBmpDims((base_path + "1.bmp").c_str());
    const size_t pix_mem_img_sz = img_dims.x * img_dims.y * 4;
    sf::Uint8* frame_pixel_buffer = (sf::Uint8*)malloc(pix_mem_img_sz * n_images);
    LoadBmpSequenceToMemory(frame_pixel_buffer, base_path, 1, n_images);

    
    uint32_vec2d track_position_center(500, 300);
    uint32_vec2d track_size(30, 30);    

    sf::Uint8* track_template_pixels = (sf::Uint8*)malloc(track_size.x * track_size.y * 4);

    PixelRegion template_pixel_region(
        track_position_center.x, track_position_center.y,
        track_size.x, track_size.y, img_dims.x, img_dims.y);

    extract_region_pixels(frame_pixel_buffer, track_template_pixels, template_pixel_region);

    d_draw_pixels(track_template_pixels, template_pixel_region.region_width, template_pixel_region.region_height);


    sf::Texture current_frame_preview_tex;
    sf::Sprite current_frame_preview_sprite;
    double frame_scaling


    current_frame_preview_tex.create(img_dims.x, img_dims.y);
    //Implictily loads the first image
    current_frame_preview_tex.update(frame_pixel_buffer);

   


    sf::RenderWindow window(sf::VideoMode(
        sf::VideoMode::getDesktopMode().width * 0.6,
        sf::VideoMode::getDesktopMode().height * 0.6),
        "XTracker - Optimized");



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



        window.display();
    }

    free(frame_pixel_buffer);
}



int main(int argc, char** argv)
{
   // if (argc < 2) std::cout << "Not enough arguments, use -help for usage" << std::endl;

    XTrackerMain();

    return 0;

    if (std::string(argv[1]) == "-h")
    {
        std::cout << "--- XTracker Command Documentation ----\n"
            "-h Open this menu for help\n"
            "-i [PATH] (Required) Input file, PATH can be relative or absolute\n\n"
            "-o [NAME] (Required) Data output file name AND extension. i.e.: \"track_data.csv\"\n\n"
            "-s [SCALING] (Optional, default=0.6) XTracker window scaling, 1 fills the display, 0.5 uses half and so on\n\n"
            "-d [DELIMITER] (Optional, default=,) Data column delimiter default format is CSV\n\n"
            "-u [ANGLE UNITS] (Optional, default=deg) Angle units for export, can be either \"deg\" or \"rad\"\n\n"
            "-m [MAX MEMORY(MB)] (Optional, default=2000) Max RAM and disk usage, higher values may improve performance\n\n"
            "-p [PRECISION(int)] (Optional, default=1) Pixel tracking precision, higher values proportionally increase performance, and decrease precision\n\n";
            "-dev ONLY USED BY DEVS, IT WILL ERROR FOR YOU!\n\n";
    }

    


   // XTrackerMain();

    return 0;

}