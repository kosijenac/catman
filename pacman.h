#pragma once
#include "screen.h"
#include "textbox.h"
#include <SFML/Graphics.hpp>
#include <deque>
#include <iostream>
#include <random>
#include <string>

enum class Direction { Up,
    Down,
    Left,
    Right };

class Pacman {
public:
    // konstruktor i destruktor
    Pacman(int, Textbox*);
    ~Pacman() {};

    void Reset();
    void Step();
    void Move();
    void DetectCollision();
    void Render(Screen*);

    void SetDirection(Direction d) { dir = d; }
    Direction GetDirection() { return dir; }
    float GetSpeed() { return speed; }
    sf::Vector2i GetCoords() { return coords.front(); }
    int GetLives() { return lives; }
    int GetPoints() { return points; }
    bool isGameOver() { return gameOver; }
    void incPoints() { points += 10; }
    void endGame()
    {
        textbox->Append("Igra gotova. Ukupni points: " + std::to_string(GetPoints()));
        gameOver = true;
    }

private:
    // coords blockova od kojih se sastoji tijelo
    std::deque<sf::Vector2i> coords;
    Direction dir;
    int lives;
    int points;
    bool gameOver;
    float speed;
    int unitSize;
    sf::RectangleShape block;
    Textbox* textbox;
};

class Map {
public:
    Map(int, sf::Vector2u, Textbox*);
    ~Map() {};
    void SetJabuku();
    void Update(Pacman&);
    int dohvatiVBloka() { return velicinaBloka; }
    void Render(Screen*);

private:
    sf::Vector2u velicinaScreena;
    int velicinaBloka;
    sf::Vector2i jabukaKoord;
    sf::CircleShape jabuka;
    sf::RectangleShape rub;
    Textbox* ptextbox;
};