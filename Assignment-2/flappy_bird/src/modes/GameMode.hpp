#pragma once

#include <SFML/Graphics.hpp>

#include <src/Bird.hpp>
#include <src/World.hpp>

class GameMode
{
public:
    enum class Mode { NORMAL, HARD };

    GameMode(std::shared_ptr<World> _world, std::shared_ptr<Bird> _bird) : world{_world}, bird{_bird} {}

    virtual ~GameMode() {}

    virtual void handle_inputs(const sf::Event & event) noexcept {}

    virtual void update(float dt) noexcept {}

    virtual void render(sf::RenderTarget & target) noexcept {}

protected:
    std::shared_ptr<Bird> bird;
    std::shared_ptr<World> world;
};