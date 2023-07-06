#include "pacman.h"
#include <fstream>
#include <vector>

bool Movable::tick = true;

Pacman::Pacman(int v, Textbox* t, sf::Texture* tx)
{
    highScore = 0;
    unitSize = v;
    textbox = t;
    texture = tx;
    sprite.setTexture(*texture);
    sprite.setScale(float(v) / 32, float(v) / 32);
    color = sf::Color::Yellow;
    Reset();
}

void Pacman::Reset(bool over)
{
    coords = sf::Vector2i(14, 23);
    SetDirection(Direction::Left);
    speed = 7;
    if (points > highScore)
        highScore = points;
    if (over) {
        lives = 3;
        points = 0;
        gameOver = false;
    }
}

void Pacman::Step()
{
    if (speed <= 14)
        speed += 0.01f;
    Move();
}

sf::Vector2i ApplyDirection(Direction d)
{
    if (d == Direction::Up)
        // To enable the bug that exists in the OG game, replace (0,-1) with (-1,-1).
        return sf::Vector2i(0, -1);
    else if (d == Direction::Down)
        return sf::Vector2i(0, 1);
    else if (d == Direction::Left)
        return sf::Vector2i(-1, 0);
    else if (d == Direction::Right)
        return sf::Vector2i(1, 0);
}

sf::IntRect GetCat(bool tick, Direction dir)
{
    if (dir == Direction::Up)
        return tick ? sf::IntRect(32, 64, 32, 32) : sf::IntRect(32, 96, 32, 32);
    else if (dir == Direction::Down)
        return tick ? sf::IntRect(192, 96, 32, 32) : sf::IntRect(224, 64, 32, 32);
    else if (dir == Direction::Left)
        return tick ? sf::IntRect(128, 64, 32, 32) : sf::IntRect(128, 96, 32, 32);
    else if (dir == Direction::Right)
        return tick ? sf::IntRect(96, 0, 32, 32) : sf::IntRect(96, 32, 32, 32);
}

void Pacman::Move() { coords += ApplyDirection(dir); }

void Pacman::Render(Screen* p)
{
    sprite.setTextureRect(GetCat(Movable::getTick(), dir));
    sprite.setColor(color);
    sprite.setPosition(coords.x * unitSize, coords.y * unitSize);
    p->draw(sprite);
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

double v2iLength(sf::Vector2i v) { return std::sqrt(v.x * v.x + v.y * v.y); }

bool Ghost::chase = false;

Ghost::Ghost(char name, sf::Texture* tx, int v)
{
    unitSize = v;
    texture = tx;
    strategy = name;
    sprite.setTexture(*texture);
    sprite.setScale(float(v) / 32, float(v) / 32);
    switch (name) {
    case 'b':
        color = sf::Color::Red;
        blinky_coords = start_coords = sf::Vector2i(14, 11);
        ghoul = sf::Vector2i(25, -2);
        break;
    case 'p':
        color = sf::Color(255, 192, 203);
        start_coords = sf::Vector2i(14, 14);
        ghoul = sf::Vector2i(2, -2);
        break;
    case 'i':
        color = sf::Color::Cyan;
        start_coords = sf::Vector2i(12, 14);
        ghoul = sf::Vector2i(27, 31);
        break;
    case 'c':
        color = sf::Color(255, 165, 0);
        start_coords = sf::Vector2i(16, 14);
        ghoul = sf::Vector2i(0, 31);
        break;
    default:
        throw std::invalid_argument("Unknown ghost name.");
    }
    Reset();
}

void Ghost::Reset()
{
    stunned = false;
    eaten = false;
    coords = start_coords;
    dir = Direction::Up;
    speed = 10;
}

void Ghost::Move(Pacman& pacman, std::vector<Direction>& dirs)
{
    Direction next;
    if (eaten)
        next = closestToGoal(start_coords, dirs);
    else if (stunned)
        next = dirs[u(e) % dirs.size()];
    else if (Ghost::chase == false) {
        next = closestToGoal(ghoul, dirs);
    } else {
        sf::Vector2i pacmanDir = ApplyDirection(pacman.GetDirection());
        if (strategy == 'b')
            next = closestToGoal(pacman.GetCoords(), dirs);
        else if (strategy == 'p')
            next = closestToGoal(pacman.GetCoords() + 4 * pacmanDir, dirs);
        else if (strategy == 'i')
            next = closestToGoal(2 * pacman.GetCoords() + 4 * pacmanDir - blinky_coords, dirs);
        else
            next = v2iLength(coords - pacman.GetCoords()) > 8 ? closestToGoal(pacman.GetCoords(), dirs)
                                                              : closestToGoal(ghoul, dirs);
    }
    dir = next;
    coords += ApplyDirection(next);
}

void Ghost::Render(Screen* p)
{
    sprite.setTextureRect(GetCat(Movable::getTick(), dir));
    sprite.setColor(eaten ? sf::Color::White : (stunned ? sf::Color::Blue : color));
    sprite.setPosition(coords.x * unitSize, coords.y * unitSize);
    p->draw(sprite);
}

std::uniform_int_distribution<unsigned> Ghost::u(0, 12);
std::default_random_engine Ghost::e(time(0));
sf::Vector2i Ghost::blinky_coords = sf::Vector2i(14, 11);

Direction Ghost::closestToGoal(sf::Vector2i goal, std::vector<Direction>& available)
{
    double distance = 1000.0;
    Direction best;
    for (Direction option : available)
        if (v2iLength(coords + ApplyDirection(option) - goal) < distance) {
            distance = v2iLength(coords + ApplyDirection(option) - goal);
            best = option;
        }
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
        for (Ghost& g : ghosts)
            g.setStunned(true);
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
                if (!collisionImminent(g, world))
                    available.push_back(static_cast<Direction>(alt));
            }
        g.Move(pacman, available);
        if (g.GetCoords().x <= 0)
            g.SetCoords(brPolja.x + g.GetCoords().x, g.GetCoords().y);
        if (g.GetCoords().x >= brPolja.x - 1)
            g.SetCoords(g.GetCoords().x % brPolja.x, g.GetCoords().y);
        if (g.strategy == 'b')
            Ghost::setBlinkyCoords(g.GetCoords());
        if (g.GetCoords() == g.getStartCoords())
            g.setEaten(false);
        if (g.GetCoords() == pacman.GetCoords()) {
            if (g.getStunned()) {
                g.setEaten(true);
                g.setStunned(false);
            } else {
                pacman.loseLife();
                pacman.Reset(false);
                for (Ghost& h : ghosts)
                    h.Reset();
                return;
            }
        }
    }
    ptextbox->Write(pacman.GetLives(), pacman.GetPoints(), pacman.GetHighScore());
    if (pacman.GetPoints() == 2600)
        ptextbox->Write("Congrats, you won! Get eaten to start a new game!");
}

void Map::Render(Screen* p)
{
    for (int i = 0; i < world.size(); i++) {
        for (int j = 0; j < world[0].size(); j++) {
            if (world[i][j] == FOOD) {
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