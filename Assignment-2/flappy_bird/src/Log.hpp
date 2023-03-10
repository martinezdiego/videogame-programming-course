/*
    ISPPJ1 2023
    Study Case: Flappy Bird

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the declaration of the class Log.
*/

#pragma once

#include <SFML/Graphics.hpp>

class Log
{
public:
    Log(float _x, float _y, bool inverted) noexcept;

    sf::FloatRect get_collision_rect() const noexcept;

    void update(float _x, float _dy = 0) noexcept;

    void render(sf::RenderTarget& target) const noexcept;

    void reset(float _x, float _y) noexcept;

private:
    float x;
    float y;
    
    bool inverted;
    sf::Sprite sprite;
};