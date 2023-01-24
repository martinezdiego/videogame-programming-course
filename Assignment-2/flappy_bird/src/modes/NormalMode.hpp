#pragma once


#include <src/modes/GameMode.hpp>

#include <src/states/StateMachine.hpp>

class NormalMode: public GameMode
{
public:
    NormalMode(Game* game);

    void handle_inputs(const sf::Event & event) noexcept override;

    void update(float dt) noexcept override;

    void render(sf::RenderTarget & target) noexcept override;

private:
    StateMachine state_machine;
};

