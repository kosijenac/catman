#include "env.h"
#include "pacman.h"
#include <SFML/Graphics.hpp>
#include <vector>

#define BLOCK_SIZE 22
#define SCREEN_WIDTH 28
#define SCREEN_HEIGHT 31

sf::Time Env::timeElapsed() { return time; }

void Env::restartClock()
{
    sf::Time diff = clock.restart();
    time += diff;
    ghostTime += diff;
}

void Env::render()
{
    p.clear();
    p.draw(spriteMap);
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
        Movable::toggleTick();
        if (pacman.isGameOver()) {
            pacman.Reset();
            for (Ghost& g : ghosts)
                g.Reset();
            map = Map(BLOCK_SIZE, p.getSize(), &textbox, "world.txt");
        }
    }
    if (ghostTime.asSeconds() >= 20.0f && Ghost::getChase()) {
        ghostTime -= sf::seconds(20.0f);
        Ghost::toggleChase();
        for (Ghost& g : ghosts)
            g.setStunned(false);
    } else if (ghostTime.asSeconds() >= 7.0f && !Ghost::getChase()) {
        ghostTime -= sf::seconds(7.0f);
        Ghost::toggleChase();
        for (Ghost& g : ghosts)
            g.setStunned(false);
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
    : p("Catman", sf::Vector2u(BLOCK_SIZE * SCREEN_WIDTH, BLOCK_SIZE * SCREEN_HEIGHT))
    , map(BLOCK_SIZE, p.getSize(), &textbox, "world.txt")
    , pacman(map.getBlockSize(), &textbox, &texture)
    , ghosts({ Ghost('c', &texture), Ghost('b', &texture), Ghost('p', &texture), Ghost('i', &texture) })
{
    textureMap.loadFromFile("maze.png");
    spriteMap.setTexture(textureMap);
    spriteMap.setScale(float(SCREEN_WIDTH * BLOCK_SIZE) / 224, float(SCREEN_HEIGHT * BLOCK_SIZE) / 248);
    texture.loadFromFile("maia_oneko.gif");
    textbox.Set(16, 450, sf::Vector2f(100, 0));
    textbox.Write(3, 0, 0);
}