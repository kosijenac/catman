#include "pacman.h"

Pacman::Pacman(int v, Textbox* t)
    : unitSize(v)
    , textbox(t)
{
    block.setSize(sf::Vector2f(float(v - 1), float(v - 1)));
    Reset();
}

void Pacman::Reset()
{
    coords.clear();
    coords.push_back(sf::Vector2i(10, 10));
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
    DetectCollision();
}

void Pacman::Move()
{
    sf::Vector2i novi = GetCoords();
    coords.pop_back();
    switch (dir) {
    case Direction::Up:
        --novi.y;
        break;
    case Direction::Down:
        ++novi.y;
        break;
    case Direction::Left:
        --novi.x;
        break;
    case Direction::Right:
        ++novi.x;
    }
    coords.push_front(novi);
}

void Pacman::DetectCollision()
{
    auto velicina = coords.size();
    sf::Vector2i glava = coords[0];
    if (velicina > 4)
        for (size_t i = 1; i < velicina; ++i)
            if (coords[i] == glava)
                return;
}

void Pacman::Render(Screen* p)
{
    auto velicina = coords.size();
    for (size_t i = 0; i < velicina; ++i) {
        block.setFillColor((i == 0) ? sf::Color::Yellow : sf::Color::Green);
        block.setPosition(coords[i].x * unitSize,
            coords[i].y * unitSize);
        p->draw(block);
    }
}

Map::Map(int vBloka, sf::Vector2u vScreena, Textbox* t)
    : velicinaBloka(vBloka)
    , velicinaScreena(vScreena)
    , ptextbox(t)
{
    // postavke jabuke
    SetJabuku();
    jabuka.setFillColor(sf::Color::Red);
    jabuka.setRadius(vBloka / 2.f);

    // postavke ruba
    rub.setFillColor(sf::Color::Transparent);
    rub.setSize(sf::Vector2f(vScreena.x,
        vScreena.y));
    rub.setOutlineColor(sf::Color::Blue);
    rub.setOutlineThickness(-vBloka);
}

void Map::SetJabuku()
{
    static std::uniform_int_distribution<unsigned> u(0, 10000);
    static std::default_random_engine e(time(0));
    int maxX = (velicinaScreena.x / velicinaBloka) - 2;
    int maxY = (velicinaScreena.y / velicinaBloka) - 2;
    jabukaKoord = sf::Vector2i(u(e) % maxX + 1, u(e) % maxY + 1);
    jabuka.setPosition(jabukaKoord.x * velicinaBloka,
        jabukaKoord.y * velicinaBloka);
}

void Map::Update(Pacman& igrac)
{
    if (igrac.GetCoords() == jabukaKoord) {
        igrac.incPoints();
        ptextbox->Append(std::string("Jabuka! Trenutni points: ")
            + std::to_string(igrac.GetPoints()));
        SetJabuku();
    }
    sf::Vector2i brPolja(velicinaScreena.x / velicinaBloka,
        velicinaScreena.y / velicinaBloka);
    if (igrac.GetCoords().x <= 0 || igrac.GetCoords().y <= 0
        || (igrac.GetCoords().x >= brPolja.x - 1) || (igrac.GetCoords().y >= brPolja.y - 1))
        igrac.endGame();
}

void Map::Render(Screen* p)
{
    p->draw(rub);
    p->draw(jabuka);
}