#include <Settings.hpp>
#include <src/text_utilities.hpp>
#include <src/states/StateMachine.hpp>
#include <src/states/ModeSelectionState.hpp>

ModeSelectionState::ModeSelectionState(StateMachine* sm) noexcept
    : BaseState{sm}
{

}

void ModeSelectionState::enter(std::shared_ptr<World> _world, std::shared_ptr<Bird> bird, int score) noexcept
{
    if (_world == nullptr) {
        world = std::make_shared<World>();
    }
    else {
        world = _world;
    }
}

void ModeSelectionState::handle_inputs(const sf::Event & event) noexcept
{
    if (event.type == sf::Event::KeyPressed) 
    {
        if ((event.key.code == sf::Keyboard::Down and !option) or (event.key.code == sf::Keyboard::Up and option)) {
            option = !option;
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            state_machine->set_game_mode(option ? GameMode::Mode::HARD : GameMode::Mode::NORMAL);
            state_machine->change_state("count_down", world);
        }
    }
}

void ModeSelectionState::update(float dt) noexcept
{
    world->update(dt);
}

void ModeSelectionState::render(sf::RenderTarget & target) const noexcept
{
    int option_offset = 15;

    world->render(target);

    render_text(target, Settings::VIRTUAL_WIDTH / 2, Settings::VIRTUAL_HEIGHT / 3, "Choose A Game Mode", Settings::FLAPPY_TEXT_SIZE, "flappy", sf::Color::White, true);
    render_text(target, Settings::VIRTUAL_WIDTH / 2, 2 * Settings::VIRTUAL_HEIGHT / 3, (option ? "  Normal" : "> Normal"), Settings::MEDIUM_TEXT_SIZE, "font", (option ? sf::Color::White : sf::Color::Yellow), true);
    render_text(target, Settings::VIRTUAL_WIDTH / 2, 2 * Settings::VIRTUAL_HEIGHT / 3 + option_offset, (option ? "> Hard" : "  Hard"), Settings::MEDIUM_TEXT_SIZE, "font", (option ? sf::Color::Yellow : sf::Color::White), true);

}