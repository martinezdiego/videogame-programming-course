#include <src/modes/HardMode.hpp>

HardMode::HardMode(std::shared_ptr<World> world, std::shared_ptr<Bird> bird) :
    GameMode(world, bird)
{

}

void HardMode::handle_inputs(const sf::Event & event) noexcept
{
    
}


void HardMode::update(float dt) noexcept
{
    bird->update(dt);
    world->update(dt);
}