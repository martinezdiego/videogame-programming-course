#pragma once


#include <src/modes/GameMode.hpp>

class NormalMode: public GameMode
{
public:
    NormalMode(std::shared_ptr<World> world, std::shared_ptr<Bird> bird);

    void handle_inputs(const sf::Event & event) noexcept override;

    void update(float dt) noexcept override;

    void render(sf::RenderTarget & target) noexcept override;
};

