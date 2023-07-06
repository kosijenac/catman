#pragma once
#include "screen.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Textbox {
public:
    Textbox();
    Textbox(int, float, sf::Vector2f);
    ~Textbox();
    void Set(int, float, sf::Vector2f);
    void Write(int, int, int);
    void Clear();
    void Render(Screen*);

private:
    std::string message;
    sf::RectangleShape background;
    sf::Font font;
    sf::Text text;
};