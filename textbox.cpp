#include "textbox.h"

Textbox::Textbox()
{
    // 5 linija, 9 piksela po znaku, 200 sirina textboxa
    // i crta se na ekranu od koordinata (0,0)
    Set(5, 9, 200, sf::Vector2f(0, 0));
}

Textbox::Textbox(int brL, int velZn, float width, sf::Vector2f pozPro)
{
    Set(brL, velZn, width, pozPro);
}

Textbox::~Textbox() { Clear(); }

void Textbox::Set(int brL, int velZn, float width, sf::Vector2f pozPro)
{
    nLines = brL;
    font.loadFromFile("cmunobi.ttf");

    text.setFont(font);
    text.setString(""); // treba li ovo?
    text.setCharacterSize(velZn);
    text.setFillColor(sf::Color::White);
    text.setPosition(pozPro + sf::Vector2f(2.0f, 2.0f));

    background.setSize(sf::Vector2f(width, (brL * (velZn * 1.2f))));
    background.setFillColor(sf::Color(90, 90, 90, 90));
    background.setPosition(pozPro);
}

void Textbox::Clear() { messages.clear(); }

void Textbox::Append(std::string nova)
{
    messages.push_back(nova);
    if (messages.size() >= 6)
        messages.pop_front();
}

void Textbox::Render(Screen* p)
{
    std::string content;
    for (auto& it : messages) {
        content.append(it + "\n");
    }
    if (content != "") {
        text.setString(content);
        p->draw(background);
        p->draw(text);
    }
}