#pragma once

#include "pacman.h"
#include "screen.h"
#include "textbox.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class Env {
public:
    Env();
    ~Env() {};
    void handleInput();
    void update();
    void render();
    Screen* getScreen() { return &p; }
    sf::Time timeElapsed();
    void restartClock();

private:
    Screen p;
    Map map;
    Pacman pacman;
    sf::Clock sat;
    sf::Time vrijeme;
    Textbox textbox;
};