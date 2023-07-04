#include "env.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    Env env;
    while (!env.getScreen()->isDone()) {
        env.handleInput();
        env.update();
        env.render();
        env.restartClock();
    }
    return 0;
}
