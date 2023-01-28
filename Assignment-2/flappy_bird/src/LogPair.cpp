/*
    ISPPJ1 2023
    Study Case: Flappy Bird

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the definition of the class LogPair.
*/

#include <Settings.hpp>
#include <src/LogPair.hpp>
#include <iostream>

LogPair::LogPair(float _x, float _y, float _gap) noexcept
    : x{_x}, y{_y}, gap{_gap},
      top{x, y + Settings::LOG_HEIGHT, true},
      bottom{x, y + _gap + Settings::LOG_HEIGHT, false}
{

}

bool LogPair::collides(const sf::FloatRect& rect) const noexcept
{
    return top.get_collision_rect().intersects(rect) || bottom.get_collision_rect().intersects(rect);
}

void LogPair::update(float dt) noexcept
{
    x += -Settings::MAIN_SCROLL_SPEED * dt;
    float dy = (Settings::MAIN_SCROLL_SPEED / 2) * dt;

    if (dynamic) {
        
        if (is_closing) {
            top.update(x, dy);
            bottom.update(x, -dy);
        }
        else {
            top.update(x, -dy);
            bottom.update(x, dy);
        }

        bool collide_each_other = top.get_collision_rect().intersects(bottom.get_collision_rect());
        bool returned_to_start_pos = top.get_collision_rect().top <= y;

        if (collide_each_other) {
            Settings::sounds["hit_someting"].play();
            is_closing = false;
        }

        if (returned_to_start_pos) {
            is_closing = true;
        }
    }
    else {
        top.update(x);
        bottom.update(x);
    }
}

void LogPair::render(sf::RenderTarget& target) const noexcept
{
    top.render(target);
    bottom.render(target);
}

bool LogPair::is_out_of_game() const noexcept
{
    return x < -Settings::LOG_WIDTH;
}

bool LogPair::update_scored(const sf::FloatRect& rect) noexcept
{
    if (scored)
    {
        return false;
    }

    if (rect.left > x + Settings::LOG_WIDTH)
    {
        scored = true;
        return true;
    }

    return false;
}

void LogPair::reset(float _x, float _y) noexcept
{
    x = _x;
    y = _y;
    scored = false;
    top.reset(x, y + Settings::LOG_HEIGHT);
    bottom.reset(x, y + gap + Settings::LOG_HEIGHT);
}


void LogPair::set_dynamic(bool _dynamic) noexcept
{
    dynamic = _dynamic;
}