/*
    ISPPJ1 2023
    Study Case: Flappy Bird

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the declaration of the class Game.
*/

#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <src/modes/GameMode.hpp>

class Game
{
public:
    Game();

    Game(const Game&) = delete;

    Game& operator = (Game) = delete;

    sf::RenderWindow& get_window() noexcept;

    void handle_inputs(const sf::Event& event) noexcept;

    void update(float dt) noexcept;

    void render() noexcept;

    void change_game_mode(const std::string & mode_name) noexcept;

private:
    sf::RenderWindow render_window;
    sf::RenderTexture render_texture;
    sf::Sprite render_sprite;

    using GameModeBuilder = std::function<std::shared_ptr<GameMode>(Game*)>;
    
    std::unordered_map<std::string, GameModeBuilder> game_modes;
    std::shared_ptr<GameMode> current_game_mode{std::make_shared<GameMode>(this)};

    bool bird_is_dead{false};
    int score{0};
};
