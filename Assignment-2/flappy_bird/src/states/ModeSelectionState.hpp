#pragma once

#include <src/World.hpp>
#include <src/states/BaseState.hpp>

class ModeSelectionState: public BaseState
{
public:
    ModeSelectionState(StateMachine* sm) noexcept;

    void enter(std::shared_ptr<World> _world, std::shared_ptr<Bird> bird, int score) noexcept override;

    void handle_inputs(const sf::Event & event) noexcept override;

    void update(float dt) noexcept override;

    void render(sf::RenderTarget & target) const noexcept override;

private:
    std::shared_ptr<World> world;
    int option{0};
};

