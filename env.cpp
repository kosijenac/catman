#include "env.h"
#include "pacman.h"
#include <SFML/Graphics.hpp>
#include <vector>

#define BLOCK_SIZE 22
#define SCREEN_WIDTH 28
#define SCREEN_HEIGHT 31

sf::Time Env::timeElapsed() { return time; }

void Env::restartClock() { time += clock.restart(); }

void Env::render()
{
    p.clear();
    map.Render(&p);
    pacman.Render(&p);
    for (Ghost g : ghosts)
        g.Render(&p);
    textbox.Render(&p);
    p.display();
}

void Env::update()
{
    p.update();
    float timeIter = 1.0f / pacman.GetSpeed();
    if (time.asSeconds() >= timeIter) {
        map.Update(pacman, ghosts);
        time -= sf::seconds(timeIter);
        if (pacman.isGameOver())
            pacman.Reset();
    }
}

void Env::handleInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
        && map.getWorld()[pacman.GetCoords().y - 1][pacman.GetCoords().x] != WALL)
        pacman.SetDirection(Direction::Up);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
        && map.getWorld()[pacman.GetCoords().y + 1][pacman.GetCoords().x] != WALL)
        pacman.SetDirection(Direction::Down);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && map.getWorld()[pacman.GetCoords().y][pacman.GetCoords().x - 1] != WALL)
        pacman.SetDirection(Direction::Left);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
        && map.getWorld()[pacman.GetCoords().y][pacman.GetCoords().x + 1] != WALL)
        pacman.SetDirection(Direction::Right);
}

Env::Env()
    : p("Pacman", sf::Vector2u(BLOCK_SIZE * SCREEN_WIDTH, BLOCK_SIZE * SCREEN_HEIGHT))
    , map(BLOCK_SIZE, p.getSize(), &textbox, "world.txt")
    , pacman(map.getBlockSize(), &textbox)
    , ghosts({ 'c', 'b', 'p', 'i' })
{
    textbox.Set(16, 275, sf::Vector2f(175, 0));
    textbox.Write("Lives left: 3\t\t\tPoints: 0");
}