/*
    ISPPJ1 2023
    Study Case: Flappy Bird

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the definition of the class Game.
*/

#include <Settings.hpp>
#include <src/Game.hpp>
#include <src/modes/NormalMode.hpp>
#include <src/modes/HardMode.hpp>

Game::Game()
    : render_window{sf::VideoMode{Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT}, "Flappy Bird", sf::Style::Close},
      render_texture{},
      render_sprite{},
      game_modes{
        {"normal", [](Game* game) { return std::make_shared<NormalMode>(game); }},
        {"hard", [](Game* game) { return std::make_shared<HardMode>(game); }}
    }
{
    render_texture.create(Settings::VIRTUAL_WIDTH, Settings::VIRTUAL_HEIGHT);

    sf::Vector2f scale_factors{
        float(Settings::WINDOW_WIDTH) / float(Settings::VIRTUAL_WIDTH), 
        float(Settings::WINDOW_HEIGHT) / float(Settings::VIRTUAL_HEIGHT)
    };

    render_sprite.setTexture(render_texture.getTexture());
    render_sprite.setScale(scale_factors);

    change_game_mode("normal");

    Settings::music.setLoop(true);
    Settings::music.play();
}

sf::RenderWindow& Game::get_window() noexcept
{
    return render_window;
}

void Game::handle_inputs(const sf::Event& event) noexcept
{
    current_game_mode->handle_inputs(event);
}

void Game::update(float dt) noexcept
{
    current_game_mode->update(dt);
}

void Game::render() noexcept
{
    render_texture.clear(sf::Color::Black);
    
    current_game_mode->render(render_texture);

    render_texture.display();

    render_window.draw(render_sprite);
    render_window.display();
}

void Game::change_game_mode(const std::string & mode_name) noexcept
{
    auto it = game_modes.find(mode_name);

    if (it == game_modes.end())
    {
        return;
    }

    current_game_mode = it->second(this);
}