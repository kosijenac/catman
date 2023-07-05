#include "pacman.h"
#include <fstream>
#include <vector>

Pacman::Pacman(int v, Textbox* t)
{
    unitSize = v;
    textbox = t;
    color = sf::Color::Yellow;
    block.setRadius(float(v - 1) / 2);
    Reset();
}

void Pacman::Reset()
{
    coords = sf::Vector2i(14, 23);
    SetDirection(Direction::Left);
    speed = 10;
    lives = 3;
    points = 0;
    gameOver = false;
}

void Pacman::Step()
{
    if (speed <= 20)
        speed += 0.01f;
    Move();
}

sf::Vector2i ApplyDirection(Direction d)
{
    if (d == Direction::Up)
        return sf::Vector2i(-1, 0);
    else if (d == Direction::Down)
        return sf::Vector2i(1, 0);
    else if (d == Direction::Left)
        return sf::Vector2i(0, -1);
    else if (d == Direction::Right)
        return sf::Vector2i(0, 1);
}

void Pacman::Move() { coords += ApplyDirection(dir); }

void Pacman::Render(Screen* p)
{
    block.setFillColor(color);
    block.setPosition(coords.x * unitSize, coords.y * unitSize);
    p->draw(block);
}

bool collisionImminent(Movable& guy, std::vector<std::vector<char>>& w)
{
    if (guy.GetDirection() == Direction::Up && w[guy.GetCoords().y - 1][guy.GetCoords().x] == WALL)
        return true;
    if (guy.GetDirection() == Direction::Down && w[guy.GetCoords().y + 1][guy.GetCoords().x] == WALL)
        return true;
    if (guy.GetDirection() == Direction::Left && w[guy.GetCoords().y][guy.GetCoords().x - 1] == WALL)
        return true;
    if (guy.GetDirection() == Direction::Right && w[guy.GetCoords().y][guy.GetCoords().x + 1] == WALL)
        return true;
    return false;
}

// bool neighbours(Ghost& g1, Ghost& g2)
// {
//     int x1 = g1.GetCoords().x, y1 = g1.GetCoords().y, x2 = g2.GetCoords().x, y2 = g2.GetCoords().y;
//     if (g1.GetDirection() == Direction::Up && x1 == x2 && y2 == y1 - 1)
//         return true;
//     if (g1.GetDirection() == Direction::Down && x1 == x2 && y2 == y1 + 1)
//         return true;
//     if (g1.GetDirection() == Direction::Left && x1 == x2 + 1 && y2 == y1)
//         return true;
//     if (g1.GetDirection() == Direction::Right && x1 == x2 - 1 && y2 == y1)
//         return true;
//     return false;
// }

Ghost::Ghost(char name, int v)
{
    dir = Direction::Up;
    state = GhostState::Normal;
    unitSize = v;
    switch (name) {
    case 'b':
        color = sf::Color::Red;
        strategy = "blinky";
        blinky_coords = start_coords = sf::Vector2i(14, 11);
        break;
    case 'p':
        color = sf::Color(255, 192, 203);
        strategy = "pinky";
        start_coords = sf::Vector2i(14, 14);
        break;
    case 'i':
        color = sf::Color::Cyan;
        strategy = "inky";
        start_coords = sf::Vector2i(12, 14);
        break;
    case 'c':
        color = sf::Color(255, 165, 0);
        strategy = "clyde";
        start_coords = sf::Vector2i(16, 14);
        break;
    default:
        throw std::invalid_argument("Unknown ghost name.");
    }
    block.setRadius(float(v - 1) / 2);
    Reset();
}

void Ghost::Reset()
{
    coords = start_coords;
    dir = Direction::Up;
    speed = 10;
}

void Ghost::Move(Pacman& pacman, std::vector<Direction>& available)
{
    Direction next;
    if (state == GhostState::Stunned)
        next = stunned(available);
    else if (state == GhostState::Eaten)
        next = closestToGoal(start_coords, available)
}

void Ghost::Render(Screen* p)
{
    block.setFillColor(state == GhostState::Stunned ? sf::Color::Blue : color);
    block.setPosition(coords.x * unitSize, coords.y * unitSize);
    p->draw(block);
}

std::uniform_int_distribution<unsigned> Ghost::u(0, 12);
std::default_random_engine Ghost::e(time(0));

double v2iLength(sf::Vector2i v) { return std::sqrt(v.x * v.x + v.y * v.y); }

Direction Ghost::stunned(std::vector<Direction>& av) { return av[u(e) & av.size()]; }

Direction Ghost::blinky() { return Direction::Up; }

Direction Ghost::pinky() { return Direction::Up; }

Direction Ghost::inky() { return Direction::Up; }

Direction Ghost::clyde() { return Direction::Up; }

Direction Ghost::closestToGoal(sf::Vector2i goal, std::vector<Direction>& available)
{
    double distance = 1000.0;
    Direction best;

    // for (Direction av : available) {
    //     switch (av) {
    //     case Direction::Up:
    //         if (v2iLength(goal -))
    //     }
    // }
    return best;
}

Map::Map(int vBloka, sf::Vector2u size, Textbox* t, std::string filename)
    : blockSize(vBloka)
    , screenSize(size)
    , ptextbox(t)
{
    std::ifstream fs(filename);
    std::string line;
    while (getline(fs, line))
        world.push_back(std::vector<char>(line.begin(), line.end()));
}

void Map::Update(Pacman& pacman, std::vector<Ghost>& ghosts)
{
    if (!collisionImminent(pacman, world))
        pacman.Step();

    if (world[pacman.GetCoords().y][pacman.GetCoords().x] == FOOD) {
        pacman.incPoints();
        world[pacman.GetCoords().y][pacman.GetCoords().x] = BLANK;
    } else if (world[pacman.GetCoords().y][pacman.GetCoords().x] == BALL) {
        pacman.incPoints(50);
        for (Ghost g : ghosts)
            g.setState(GhostState::Stunned);
        world[pacman.GetCoords().y][pacman.GetCoords().x] = BLANK;
    }

    sf::Vector2i brPolja(screenSize.x / blockSize, screenSize.y / blockSize);
    if (pacman.GetCoords().x <= 0)
        pacman.SetCoords(brPolja.x + pacman.GetCoords().x, pacman.GetCoords().y);
    if (pacman.GetCoords().x >= brPolja.x - 1)
        pacman.SetCoords(pacman.GetCoords().x % brPolja.x, pacman.GetCoords().y);

    for (Ghost& g : ghosts) {
        Direction current = g.GetDirection();
        std::vector<Direction> available;
        for (int alt = 0; alt < 4; alt++)
            if (static_cast<Direction>(3 - alt) != current) {
                g.SetDirection(static_cast<Direction>(alt));
                // bool hasNeighbour = false;
                // for (Ghost& other : ghosts)
                //     if (other != g && neighbours(g, other))
                //         hasNeighbour = true;
                if (!collisionImminent(g, world))
                    available.push_back(static_cast<Direction>(alt));
            }
        g.Move(pacman, available);
    }

    ptextbox->Write("Lives left: " + std::to_string(pacman.GetLives())
        + "\t\t\tPoints: " + std::to_string(pacman.GetPoints()));
}

void Map::Render(Screen* p)
{
    for (int i = 0; i < world.size(); i++) {
        for (int j = 0; j < world[0].size(); j++) {
            if (world[i][j] == WALL) {
                sf::RectangleShape wall;
                wall.setSize(sf::Vector2f(float(blockSize), float(blockSize)));
                wall.setFillColor(sf::Color::Blue);
                wall.setPosition(j * blockSize, i * blockSize);
                p->draw(wall);
            } else if (world[i][j] == FOOD) {
                sf::CircleShape food;
                food.setRadius(2);
                food.setFillColor(sf::Color::White);
                food.setPosition((j + 0.5) * blockSize - 2, (i + 0.5) * blockSize - 2);
                p->draw(food);
            } else if (world[i][j] == BALL) {
                sf::CircleShape food;
                food.setRadius(7);
                food.setFillColor(sf::Color::White);
                food.setPosition((j + 0.5) * blockSize - 7, (i + 0.5) * blockSize - 7);
                p->draw(food);
            }
        }
    }
}