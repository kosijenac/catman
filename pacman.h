#pragma once
#include "screen.h"
#include "textbox.h"
#include <SFML/Graphics.hpp>
#include <deque>
#include <iostream>
#include <random>
#include <string>

#define WALL '#'
#define BALL 'o'
#define FOOD '.'
#define BLANK ' '

enum class Direction { Up,
    Left,
    Right,
    Down };

enum class GhostState { Normal,
    Stunned,
    Eaten };

class Movable {
protected:
    sf::Vector2i coords;
    Direction dir;
    float speed;
    int unitSize;
    sf::CircleShape block;
    sf::Color color;

public:
    void SetDirection(Direction d) { dir = d; }
    Direction GetDirection() { return dir; }
    float GetSpeed() { return speed; }
    void SetCoords(int x, int y) { coords = sf::Vector2i(x, y); }
    sf::Vector2i GetCoords() { return coords; }
};

class Pacman : public Movable {
    int lives;
    int points;
    bool gameOver;
    Textbox* textbox;

public:
    // konstruktor i destruktor
    Pacman(int, Textbox*);
    ~Pacman() {};

    void Reset();
    void Step();
    void Move();
    void Render(Screen*);

    int GetLives() { return lives; }
    int GetPoints() { return points; }
    bool isGameOver() { return gameOver; }
    void incPoints(int n = 10) { points += n; }
    void decLives() { lives--; }
    void shiftUpDown(int n) { coords.y += n; }
    void shiftLeftRight(int n) { coords.x += n; }
    void endGame()
    {
        textbox->Write("Game over. Points scored: " + std::to_string(GetPoints()));
        gameOver = true;
    }
};

class Ghost : public Movable {
    sf::Vector2i start_coords;
    sf::Vector2i blinky_coords;
    sf::Vector2i ghoul; // like 'goal', but for ghosts :P
    GhostState state;
    Direction stunned(std::vector<Direction>&);
    Direction blinky();
    Direction pinky();
    Direction inky();
    Direction clyde();
    static std::uniform_int_distribution<unsigned> u;
    static std::default_random_engine e;

public:
    std::string strategy;
    Ghost(char, int s = 22);
    Ghost() = default;
    Ghost(const Ghost&) = default;
    Ghost(Ghost&&) = default;
    ~Ghost() = default;
    bool operator!=(Ghost& other) { return strategy != other.strategy; }
    void Reset();
    void Move(Pacman&, std::vector<Direction>&);
    void Render(Screen*);
    void setState(GhostState s) { state = s; }
    Direction closestToGoal(sf::Vector2i, std::vector<Direction>&);
};

class Map {
    sf::Vector2u screenSize;
    int blockSize;
    std::vector<std::vector<char>> world;
    Textbox* ptextbox;

public:
    Map(int, sf::Vector2u, Textbox*, std::string);
    ~Map() {};
    void Update(Pacman&, std::vector<Ghost>&);
    std::vector<std::vector<char>>& getWorld() { return world; }
    int getBlockSize() { return blockSize; }
    void Render(Screen*);
};

bool collisionImminent(Movable&, Map&);