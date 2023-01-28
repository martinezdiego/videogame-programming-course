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
    score = _score;

    if (_bird == nullptr)
    {
        bird = std::make_shared<Bird>(
            Settings::VIRTUAL_WIDTH / 2 - Settings::BIRD_WIDTH / 2, Settings::VIRTUAL_HEIGHT / 2 - Settings::BIRD_HEIGHT / 2,
            Settings::BIRD_WIDTH, Settings::BIRD_HEIGHT
        );
        world->reset(true);
    }
    else
    {
        bird = _bird;
        
        if (bird->get_invincible()) {
            Settings::powerup_music.play();
        }
        else {
            Settings::music.play();
        }
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

    if (bird->get_invincible()) 
    {
        powerup_limit_timer += dt;
        
        if (powerup_limit_timer >= Settings::POWERUP_TIME_LIMIT) 
        {    
            powerup_limit_timer = 0.f;
            bird->set_invincible(false);
            world->reset(true);
            Settings::powerup_music.stop();
            Settings::sounds["leave_powerup"].play();
            Settings::music.play();
        }
    }

    if (world->collides_with_ground(bird->get_collision_rect()))
    {
        Settings::sounds["explosion"].play();
        Settings::sounds["hurt"].play();
        Settings::powerup_music.stop();

        auto music_status = Settings::music.getStatus();
        
        if (music_status == sf::Music::Status::Paused) {
            Settings::music.play();
        }
        
        state_machine->change_state("count_down");
    }
    else if (!bird->get_invincible() && world->collides_with_logs(bird->get_collision_rect()))
    {
        Settings::sounds["explosion"].play();
        Settings::sounds["hurt"].play();
        state_machine->change_state("count_down");
    }
    else if (world->update_scored(bird->get_collision_rect()))
    {
        ++score;
        Settings::sounds["score"].play();
    }
    else if (world->collides_with_powerup(bird->get_collision_rect())) 
    {
        Settings::music.pause();
        Settings::sounds["get_powerup"].play();
        Settings::powerup_music.play();
        bird->set_invincible(true);
    }
}

void PlayingState::render(sf::RenderTarget& target) const noexcept
{
    world->render(target);
    bird->render(target);
    
    render_text(target, 20, 10, "Score: " + std::to_string(score), Settings::FLAPPY_TEXT_SIZE, "flappy", sf::Color::White);
}