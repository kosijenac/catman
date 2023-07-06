#include "textbox.h"

// 5 linija, 9 piksela po znaku, 200 sirina textboxa
// i crta se na ekranu od koordinata (0,0)
Textbox::Textbox() { Set(14, 300, sf::Vector2f(0, 0)); }

Textbox::Textbox(int velZn, float width, sf::Vector2f pozPro)
{
    Set(velZn, width, pozPro);
}

Textbox::~Textbox() { Clear(); }

void Textbox::Set(int velZn, float width, sf::Vector2f pozPro)
{
    font.loadFromFile("cmunobx.ttf");

    text.setFont(font);
    text.setString(""); // treba li ovo?
    text.setCharacterSize(velZn);
    text.setFillColor(sf::Color::White);
    text.setPosition(pozPro + sf::Vector2f(2.0f, 2.0f));

    background.setSize(sf::Vector2f(width, (velZn * 1.2f)));
    background.setFillColor(sf::Color(90, 90, 90, 60));
    background.setPosition(pozPro);
}

void Textbox::Clear() { message.clear(); }

void Textbox::Render(Screen* p)
{
    if (message != "") {
        text.setString(message);
        p->draw(background);
        p->draw(text);
    }
}

void Textbox::Write(int lives, int points, int score)
{
    message = "Lives left: " + std::to_string(lives) + "\t\tPoints: " + std::to_string(points)
        + "\t\tHigh Score: " + std::to_string(score);
}

void Textbox::Write(std::string txt) { message = txt; }