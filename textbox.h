#pragma once
#include "screen.h"
#include <SFML/Graphics.hpp>
#include <deque>
#include <iostream>
#include <string>

class Textbox {
public:
    Textbox();
    Textbox(int, int, float, sf::Vector2f);
    ~Textbox();
    void Set(int, int, float, sf::Vector2f);
    void Append(std::string);
    void Clear();
    void Render(Screen*);

private:
    int nLines;
    std::deque<std::string> messages;
    sf::RectangleShape background;
    sf::Font font;
    sf::Text text;
};