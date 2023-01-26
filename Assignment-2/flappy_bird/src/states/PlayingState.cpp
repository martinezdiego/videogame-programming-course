/*
    ISPPJ1 2023
    Study Case: Flappy Bird

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the definition of the class PlayingBaseState.
*/

#include <Settings.hpp>
#include <src/text_utilities.hpp>

#include <src/states/StateMachine.hpp>
#include <src/states/PlayingState.hpp>

#include <src/modes/NormalMode.hpp>
#include <src/modes/HardMode.hpp>

PlayingState::PlayingState(StateMachine* sm) noexcept
    : BaseState{sm}, game_mode_handler{nullptr}
{
    
}

void PlayingState::enter(std::shared_ptr<World> _world, std::shared_ptr<Bird> _bird, int _score) noexcept
{
    world = _world;
    world->reset(true);
    score = _score;

    if (_bird == nullptr)
    {
        bird = std::make_shared<Bird>(
            Settings::VIRTUAL_WIDTH / 2 - Settings::BIRD_WIDTH / 2, Settings::VIRTUAL_HEIGHT / 2 - Settings::BIRD_HEIGHT / 2,
            Settings::BIRD_WIDTH, Settings::BIRD_HEIGHT
        );
    }
    else
    {
        bird = _bird;
    }

    auto music_status = Settings::music.getStatus();

    if (music_status != sf::SoundSource::Status::Playing) {
        Settings::music.play();
    }

    if (game_mode_handler == nullptr) {

        auto current_game_mode = state_machine->get_game_mode();

        if (current_game_mode== GameMode::Mode::NORMAL) {
            game_mode_handler = std::make_shared<NormalMode>(world, bird);
        }
        else if (current_game_mode == GameMode::Mode::HARD) {
            game_mode_handler = std::make_shared<HardMode>(world, bird);
        }
    }
}

void PlayingState::handle_inputs(const sf::Event& event) noexcept
{
    game_mode_handler->handle_inputs(event);
    
    if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::P) {
        state_machine->change_state("pause", world, bird, score);
    }
}

void PlayingState::update(float dt) noexcept
{
    game_mode_handler->update(dt);

    if (world->collides(bird->get_collision_rect()))
    {
        Settings::sounds["explosion"].play();
        Settings::sounds["hurt"].play();
        state_machine->change_state("count_down");
    }

    if (world->update_scored(bird->get_collision_rect()))
    {
        ++score;
        Settings::sounds["score"].play();
    }
}

void PlayingState::render(sf::RenderTarget& target) const noexcept
{
    world->render(target);
    bird->render(target);
    
    render_text(target, 20, 10, "Score: " + std::to_string(score), Settings::FLAPPY_TEXT_SIZE, "flappy", sf::Color::White);
}