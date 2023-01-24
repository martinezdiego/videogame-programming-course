#pragma once

#include <SFML/Graphics.hpp>

class Game;

class GameMode
{
public:
    GameMode(Game* _game): game{_game} {}

    virtual ~GameMode() {}

    virtual void handle_inputs(const sf::Event & event) noexcept {}

    virtual void update(float dt) noexcept {}

    virtual void render(sf::RenderTarget & target) noexcept {}

private:
    Game* game;
};