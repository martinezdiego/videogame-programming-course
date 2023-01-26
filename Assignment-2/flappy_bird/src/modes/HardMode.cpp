#include <src/modes/HardMode.hpp>

HardMode::HardMode(std::shared_ptr<World> world, std::shared_ptr<Bird> bird) :
    GameMode(world, bird)
{

}

void HardMode::handle_inputs(const sf::Event & event) noexcept
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        bird->jump();
    }
    
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
        bird->move_left();
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
        bird->move_right();
    }
}


void HardMode::update(float dt) noexcept
{
    bird->update(dt);
    world->update(dt);
}