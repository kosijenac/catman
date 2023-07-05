#include "screen.h"

void Screen::update()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            done = true;
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5)
            toggleFullScreen();
    }
}

void Screen::toggleFullScreen()
{
    fullScreen = !fullScreen;
    Destroy();
    Spawn();
}

Screen::Screen(const std::string& n, const sf::Vector2u& v) { Assign(n, v); }

Screen::Screen() { Assign("Pacman", sf::Vector2u(640, 480)); }

Screen::~Screen() { Destroy(); }

void Screen::Assign(const std::string& n, const sf::Vector2u& v)
{
    title = n;
    size = v;
    fullScreen = false;
    done = false;
    // otvaranje windowa nakon popunjavanja postavki
    Spawn();
}

void Screen::Spawn()
{
    auto style = (fullScreen ? sf::Style::Fullscreen : sf::Style::Default);
    window.create(sf::VideoMode(size.x, size.y, 32), title, style);
}

void Screen::Destroy() { window.close(); }

void Screen::clear() { window.clear(sf::Color::Black); }

void Screen::draw(sf::Drawable& d) { window.draw(d); }

void Screen::display() { window.display(); }