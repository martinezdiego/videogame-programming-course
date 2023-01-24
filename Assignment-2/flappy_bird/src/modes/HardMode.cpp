#include <src/modes/HardMode.hpp>

#include <src/states/CountDownState.hpp>
#include <src/states/TitleScreenState.hpp>
#include <src/states/PlayingState.hpp>
#include <src/states/PauseState.hpp>

HardMode::HardMode(Game* game) :
    GameMode(game),
    state_machine{
        {"title", [](StateMachine* sm) { return std::make_shared<TitleScreenState>(sm); }},
        {"count_down", [](StateMachine* sm) { return std::make_shared<CountDownState>(sm); }},
        {"playing", [](StateMachine* sm) { return std::make_shared<PlayingState>(sm); }},
        {"pause", [](StateMachine* sm) { return std::make_shared<PauseState>(sm); }}
        
    }
{
    state_machine.change_state("title");
}

void HardMode::handle_inputs(const sf::Event & event) noexcept
{
    state_machine.handle_inputs(event);
}


void HardMode::update(float dt) noexcept
{
    state_machine.update(dt);
}

void HardMode::render(sf::RenderTarget & target) noexcept
{
    state_machine.render(target);
}