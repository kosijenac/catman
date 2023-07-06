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

class Movable {
protected:
    sf::Vector2i coords;
    Direction dir;
    float speed;
    int unitSize;
    sf::Color color;
    static bool tick;

public:
    sf::Sprite sprite;
    sf::Texture* texture;
    void SetDirection(Direction d) { dir = d; }
    Direction GetDirection() { return dir; }
    float GetSpeed() { return speed; }
    void SetCoords(int x, int y) { coords = sf::Vector2i(x, y); }
    sf::Vector2i GetCoords() { return coords; }
    static bool getTick() { return tick; }
    static void toggleTick() { tick = !tick; }
};

class Pacman : public Movable {
    int lives;
    int points;
    int highScore;
    bool gameOver;
    Textbox* textbox;

public:
    Pacman(int, Textbox*, sf::Texture*);
    ~Pacman() {};

    void Reset(bool over = true);
    void Step();
    void Move();
    void Render(Screen*);

    int GetLives() { return lives; }
    int GetPoints() { return points; }
    int GetHighScore() { return highScore; }
    bool isGameOver() { return gameOver; }
    void incPoints(int n = 10) { points += n; }
    void loseLife() { gameOver = (lives-- == 0) ? true : gameOver; }
    void endGame() { gameOver = true; }
};

class Ghost : public Movable {
    sf::Vector2i start_coords;
    static sf::Vector2i blinky_coords;
    sf::Vector2i ghoul; // like 'goal', but for ghosts :P
    bool eaten;
    bool stunned;
    static std::uniform_int_distribution<unsigned> u;
    static std::default_random_engine e;
    static bool chase;

public:
    char strategy;
    Ghost(char, sf::Texture*, int s = 22);
    ~Ghost() {};
    bool operator!=(Ghost& other) { return strategy != other.strategy; }
    void Reset();
    void Move(Pacman&, std::vector<Direction>&);
    void Render(Screen*);
    void setEaten(bool e) { eaten = e; }
    sf::Vector2i getStartCoords() { return start_coords; }
    void setStunned(bool s) { stunned = s; }
    bool getStunned() { return stunned; }
    static void setBlinkyCoords(sf::Vector2i c) { blinky_coords = c; }
    static void toggleChase() { chase = !chase; }
    static bool getChase() { return chase; }
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