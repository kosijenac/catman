#include "env.h"
#include <SFML/Graphics.hpp>

sf::Time Env::timeElapsed() { return vrijeme; }

void Env::restartClock() { vrijeme += sat.restart(); }

void Env::render()
{
    p.clear();
    map.Render(&p);
    pacman.Render(&p);
    textbox.Render(&p);
    p.display();
}

void Env::update()
{
    p.update();
    float vrijemeIteracije = 1.0f / pacman.GetSpeed();
    if (vrijeme.asSeconds() >= vrijemeIteracije) {
        pacman.Step();
        map.Update(pacman);
        vrijeme -= sf::seconds(vrijemeIteracije);
        if (pacman.isGameOver()) {
            pacman.Reset();
        }
    }
}

void Env::handleInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        pacman.SetDirection(Direction::Up);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        pacman.SetDirection(Direction::Down);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        pacman.SetDirection(Direction::Left);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        pacman.SetDirection(Direction::Right);
}

Env::Env()
    : p("Pacman", sf::Vector2u(800, 640))
    , map(16, p.getSize(), &textbox)
    , pacman(map.dohvatiVBloka(), &textbox)
{
    textbox.Set(5, 14, 350, sf::Vector2f(225, 0));
    textbox.Append("Dobro dosli u igru Pacman!");
}