#include <SFML/Graphics.hpp>

#include <nlohmann/json.hpp> 
using json = nlohmann::json;

#include <iostream>
#include <fstream>

#include "Platform.h"
#include "Game.h"

int main() {

    std::ifstream file("../config/settings.json");
    json settings;
    file >> settings;

    unsigned int window_width = settings["window"]["width"];
    unsigned int window_height = settings["window"]["height"];
    std::string window_title = settings["window"]["title"];
    unsigned int framerate = settings["window"]["framerate"];


    sf::RenderWindow window(sf::VideoMode(window_width, window_height), window_title);
    window.setFramerateLimit(framerate);
    
    Game game(window);
    game.init();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            game.handleEvent(event);

        }

        game.update();
        window.clear(sf::Color::Black);
        game.render();
        window.display();
    }

    return 0;
}