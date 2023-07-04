#pragma once

#include <SFML/Graphics.hpp>

class Screen {
    void Spawn();
    void Destroy();
    void Assign(const std::string&, const sf::Vector2u&);
    sf::RenderWindow window;
    sf::Vector2u size;
    std::string title;
    bool done;
    bool fullScreen;

public:
    Screen();
    Screen(const std::string&, const sf::Vector2u&);
    void toggleFullScreen();
    void clear();
    void draw(sf::Drawable&);
    void display();
    void update();
    ~Screen();
    bool isDone() { return done; }
    bool isFullScreen() { return fullScreen; }
    sf::Vector2u getSize() { return size; }
};
