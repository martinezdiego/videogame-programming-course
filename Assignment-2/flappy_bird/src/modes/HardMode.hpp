#pragma once

#include <src/modes/GameMode.hpp>

class HardMode: public GameMode
{
public:
    HardMode(std::shared_ptr<World> world, std::shared_ptr<Bird> bird);

    void handle_inputs(const sf::Event & event) noexcept override;

    void update(float dt) noexcept override;
};

