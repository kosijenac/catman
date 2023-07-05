#pragma once

#include "pacman.h"
#include "screen.h"
#include "textbox.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

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
    std::vector<Ghost> ghosts;
    sf::Clock clock;
    sf::Time time;
    Textbox textbox;
};