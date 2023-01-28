#pragma once

#include <SFML/Graphics.hpp>

class PowerUp
{
public:

    PowerUp(float _x, float _y) noexcept;

    sf::FloatRect get_collision_rect() const noexcept;

    bool is_out_of_game() const noexcept;

    void update(float dt) noexcept;

    void render(sf::RenderTarget& target) const noexcept;

    void reset(float _x, float _y) noexcept;

private:
    
    float x;
    float y;
    sf::Sprite sprite;
};