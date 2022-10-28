
#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "ImageFileProcessing.h"
#include <string>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


int main(int argc, char** argv)
{
    std::string base_path = "C:/Users/Aspen/Desktop/XTracker/BmpFrames/output";
    size_t n_images = 100;
    auto img_dims = GetBmpDims((base_path + "1.bmp").c_str());
    auto t1 = high_resolution_clock::now();

    const size_t pix_mem_img_sz = img_dims.x * img_dims.y * 4;



    sf::Uint8 *frame_pixel_buffer = (sf::Uint8*)malloc(pix_mem_img_sz * n_images);



    LoadBmpSequenceToMemory(frame_pixel_buffer, base_path, 1, n_images);
    //ReadBMP_To_RGBA_Ptr(frame_pixel_buffer, (base_path + "1.bmp").c_str(), 0);



    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() << "ms for " << n_images << " frames\n";




    sf::Texture bmp_tex;
    bmp_tex.create(img_dims.x, img_dims.y);
    bmp_tex.update(frame_pixel_buffer + pix_mem_img_sz);



    sf::Sprite bmp_sprite;
    bmp_sprite.setTexture(bmp_tex);

    bmp_sprite.setScale(0.6, 0.6);



    sf::RenderWindow window(sf::VideoMode(800, 600), "XTracker - Optimized");

    uint32_t i = 0;

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

        if (i == n_images) i = 0;

        bmp_tex.update(frame_pixel_buffer + pix_mem_img_sz * i);
        bmp_sprite.setTexture(bmp_tex);
        i++;

        window.clear();
        window.draw(bmp_sprite);
        window.display();
    }

    free(frame_pixel_buffer);

    return 0;

}