#include <iostream>
#include <string>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "ImageFileProcessing.h"
#include "ImageDebuggingTools.h"
#include "ImageAnalysis.h"
#include "ApplicationInterface.h"


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;



void XTrackerMain()
{

    //load images into buffer
    std::string base_path = "C:/Users/Aspen/Desktop/XTracker/BmpFrames/frame";
    size_t n_images = 100;
    auto img_dims = GetBmpDims((base_path + "1.bmp").c_str());
    const size_t pix_mem_img_sz = img_dims.x * img_dims.y * 4;
    sf::Uint8* frame_pixel_buffer = (sf::Uint8*)malloc(pix_mem_img_sz * n_images);
    LoadBmpSequenceToMemory(frame_pixel_buffer, base_path, 1, n_images);



    sf::RenderWindow window(sf::VideoMode(
        sf::VideoMode::getDesktopMode().width * 0.6,
        sf::VideoMode::getDesktopMode().height * 0.6),
        "XTracker - Optimized");
    //window.setFramerateLimit(30);


    uint32_vec2d track_position_center(500, 300);
    uint32_vec2d track_size(40, 40);    
    uint32_vec2d search_size(200, 200);    




    sf::Uint8* track_template_pixels = (sf::Uint8*)malloc(track_size.x * track_size.y * 4);

    PixelRegion template_pixel_region(
        track_position_center.x, track_position_center.y,
        track_size.x, track_size.y, img_dims.x, img_dims.y);

    extract_region_pixels(frame_pixel_buffer, track_template_pixels, template_pixel_region);

    


    sf::Texture current_frame_preview_tex;
    sf::Sprite current_frame_preview_sprite;
 


    current_frame_preview_tex.create(img_dims.x, img_dims.y);
    //Implictily loads the first image
    current_frame_preview_tex.update(frame_pixel_buffer);
    current_frame_preview_sprite.setTexture(current_frame_preview_tex);
   
    float frame_sprite_scaling = get_fit_sprite_scaling(
        window.getSize().x, window.getSize().y,
        img_dims.x, img_dims.y, 0.6, 0.6);

    current_frame_preview_sprite.setScale(frame_sprite_scaling, frame_sprite_scaling);
   // std::cout << "Sprite Scaling: " << frame_sprite_scaling << std::endl;

    size_t current_frame_index = 0;



    bool CTRL_RIGHT_pressed = false;
    bool CTRL_SPACE_pressed = false;
    bool CTRL_CLICK_pressed = false;
    bool track_playback_toggle = false;

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




        //sf::Keyboard::Key::
        if ((sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            if (CTRL_CLICK_pressed == false)
            {
                sf::Vector2i window_mouse_position = sf::Mouse::getPosition(window);

                track_position_center.x = (float)window_mouse_position.x / frame_sprite_scaling;
                track_position_center.y = (float)window_mouse_position.y / frame_sprite_scaling;

                template_pixel_region = PixelRegion(
                    track_position_center.x, track_position_center.y,
                    track_size.x, track_size.y, img_dims.x, img_dims.y);

                extract_region_pixels(frame_pixel_buffer + current_frame_index * pix_mem_img_sz, track_template_pixels, template_pixel_region);


            }
            CTRL_CLICK_pressed = true;
        }
        else CTRL_CLICK_pressed = false;





        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
            && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            if (CTRL_RIGHT_pressed == false)
            {
                if (current_frame_index + 1 < n_images) current_frame_index++;
                current_frame_preview_tex.update(frame_pixel_buffer + pix_mem_img_sz * current_frame_index);

                //d_draw_pixels(track_template_pixels, template_pixel_region.region_width, template_pixel_region.region_height);
                auto t1 = high_resolution_clock::now();

                track_position_center = search_region_for_match(
                    frame_pixel_buffer + pix_mem_img_sz * current_frame_index,
                    track_template_pixels, track_position_center.x, track_position_center.y,
                    search_size.x, search_size.y, img_dims.x, img_dims.y, track_size.x, track_size.y);

                auto t2 = high_resolution_clock::now();
                auto ms_int = duration_cast<milliseconds>(t2 - t1);
                duration<double, std::milli> ms_double = t2 - t1;

                std::cout << ms_double.count() << "ms tracking into next frame" << " frames\n";

                template_pixel_region = PixelRegion(
                    track_position_center.x, track_position_center.y,
                    track_size.x, track_size.y, img_dims.x, img_dims.y);

                extract_region_pixels(frame_pixel_buffer + current_frame_index * pix_mem_img_sz, track_template_pixels, template_pixel_region);


            }
            CTRL_RIGHT_pressed = true;
        }
        else CTRL_RIGHT_pressed = false;
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)
            && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            if (CTRL_SPACE_pressed == false)
            {
                track_playback_toggle ^= 1;
            }
            CTRL_SPACE_pressed = true;
        }
        else CTRL_SPACE_pressed = false;


        if (track_playback_toggle)
        { 
            if (current_frame_index + 1 < n_images)
            {
                //if (current_frame_index > n_images - 10) std::cout << "Frame: " << current_frame_index << std::endl;
                current_frame_index++;
            }
            current_frame_preview_tex.update(frame_pixel_buffer + pix_mem_img_sz * current_frame_index);


            auto t1 = high_resolution_clock::now();
           
            track_position_center = search_region_for_match(
                frame_pixel_buffer + pix_mem_img_sz * current_frame_index,
                track_template_pixels, track_position_center.x, track_position_center.y,
                search_size.x, search_size.y, img_dims.x, img_dims.y, track_size.x, track_size.y);

            auto t2 = high_resolution_clock::now();
            auto ms_int = duration_cast<milliseconds>(t2 - t1);
            duration<double, std::milli> ms_double = t2 - t1;

            std::cout << ms_double.count() << "ms tracking into next frame" << " frames\n";

            //perform tracking of some kind
        }


        window.clear();
        window.draw(current_frame_preview_sprite);



        draw_rectangle_border(window, frame_sprite_scaling,
            track_position_center.x, track_position_center.y,
            track_size.x, track_size.y, 3, sf::Color::Red);

        
        draw_rectangle_border(window, frame_sprite_scaling,
            track_position_center.x, track_position_center.y,
            search_size.x, search_size.y, 3, sf::Color::Blue);

        

        
        window.display();




    }

    free(frame_pixel_buffer);
}



int main(int argc, char** argv)
{
   // if (argc < 2) std::cout << "Not enough arguments, use -help for usage" << std::endl;

    
    //ffmpeg_async_call_video2bmp(
    //    "C:/Users/Aspen/Desktop/XTracker/ffmpeg/bin/ffmpeg.exe",
    //    "C:/Users/Aspen/Desktop/XTracker/SampleVideos/Pendulum2Min.mp4",
    //    "C:/Users/Aspen/Desktop/XTracker/BmpFrames/", 0, 100);

     
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