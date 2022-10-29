#pragma once
#include <SFML/Graphics.hpp>

void d_draw_pixels(const sf::Uint8 *pixels, uint32_t width, uint32_t height)
{
    sf::Texture img_tex;
    img_tex.create(width, height);
    img_tex.update(pixels); 
    sf::Sprite img_sprite;
    img_sprite.setScale(800.f / width, 800.f / height);
    img_sprite.setTexture(img_tex);

    sf::RenderWindow window(sf::VideoMode(800, 800), "XTracker Debug Window");
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)window.close();
            if (event.type == sf::Event::KeyPressed) if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }
        window.clear();
        window.draw(img_sprite);
        window.display();
    }
}