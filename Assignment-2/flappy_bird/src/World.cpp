/*
    ISPPJ1 2023
    Study Case: Flappy Bird

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the definition of the class World.
*/

#include <chrono>

#include <Settings.hpp>
#include <src/World.hpp>

World::World(bool _generate_logs) noexcept
    : generate_logs{_generate_logs}, background{Settings::textures["background"]}, ground{Settings::textures["ground"]},
      logs{}, rng{std::default_random_engine{}()}, logs_spawn_time{Settings::TIME_TO_SPAWN_LOGS}
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng.seed(seed);

    ground.setPosition(0, Settings::VIRTUAL_HEIGHT - Settings::GROUND_HEIGHT);
    std::uniform_int_distribution<int> dist(0, 80);
    last_log_y = -Settings::LOG_HEIGHT + dist(rng) + 20;
}

void World::reset(bool _generate_logs) noexcept
{
    generate_logs = _generate_logs;
}

bool World::collides(const sf::FloatRect& rect) const noexcept
{
    if (rect.top + rect.height >= Settings::VIRTUAL_HEIGHT)
    {
        return true;
    }
    
    for (auto log_pair: logs)
    {
        if (log_pair->collides(rect))
        {
            return true;
        }
    }

    return false;
}

bool World::update_scored(const sf::FloatRect& rect) noexcept
{
    for (auto log_pair: logs)
    {
        if (log_pair->update_scored(rect))
        {
            return true;
        }
    }

    return false;
}

void World::update(float dt) noexcept
{
    if (generate_logs)
    {
        logs_spawn_timer += dt;

        if (logs_spawn_timer >= Settings::TIME_TO_SPAWN_LOGS)
        {
            logs_spawn_timer = 0.f;

            std::uniform_int_distribution<int> dist{-20, 20};
            float y = std::max(-Settings::LOG_HEIGHT + 10, std::min(last_log_y + dist(rng), Settings::VIRTUAL_HEIGHT + 90 - Settings::LOG_HEIGHT));

            last_log_y = y;

            logs.push_back(log_factory.create(Settings::VIRTUAL_WIDTH, y));
        }
    }

    background_x += -Settings::BACK_SCROLL_SPEED * dt;

    if (background_x <= -Settings::BACKGROUND_LOOPING_POINT)
    {
        background_x = 0;
    }

    background.setPosition(background_x, 0);

    ground_x += -Settings::MAIN_SCROLL_SPEED * dt;

    if (ground_x <= -Settings::VIRTUAL_WIDTH)
    {
        ground_x = 0;
    }

    ground.setPosition(ground_x, Settings::VIRTUAL_HEIGHT - Settings::GROUND_HEIGHT);

    for (auto it = logs.begin(); it != logs.end(); )
    {
        if ((*it)->is_out_of_game())
        {
            auto log_pair = *it;
            log_factory.remove(log_pair);
            it = logs.erase(it);
            
        }
        else
        {
            (*it)->update(dt);
            ++it;
        }
    }
}

void World::update_hard_mode(float dt) noexcept
{
    if (generate_logs)
    {
        logs_spawn_timer += dt;
        powerup_spawn_timer += dt;

        if (logs_spawn_timer >= logs_spawn_time)
        {
            logs_spawn_timer = 0.f;
            
            std::uniform_real_distribution<float> time_dist{Settings::TIME_TO_SPAWN_LOGS, Settings::TIME_TO_SPAWN_LOGS * 2};
            std::uniform_int_distribution<int> gap_dist{0, (int) (Settings::LOGS_GAP / 2)};
            std::uniform_int_distribution<int> dist{-40, 20};
            
            int logs_gap_increment = 0;
            bool make_it_dynamic = dist(rng)%2 == 0;
            
            if (make_it_dynamic) {
                logs_gap_increment = gap_dist(rng);
            }

            float y = std::max(-Settings::LOG_HEIGHT + 10, std::min(last_log_y + dist(rng), Settings::VIRTUAL_HEIGHT + Settings::LOGS_GAP  + logs_gap_increment - Settings::LOG_HEIGHT));
            
            last_log_y = y;

            logs.push_back(log_factory.create(Settings::VIRTUAL_WIDTH, y, Settings::LOGS_GAP + logs_gap_increment));
            
            if (make_it_dynamic) {
                logs.back()->set_dynamic(true);
            }

            logs_spawn_time = time_dist(rng);
        }
        
        if (powerup_spawn_timer >= Settings::TIME_TO_SPAWN_POWERUP)
        {
            powerup_spawn_timer = 0.f;

            std::uniform_real_distribution<float> powerup_dist{Settings::POWERUP_HEIGHT, Settings::VIRTUAL_HEIGHT - Settings::POWERUP_HEIGHT};

            float y = powerup_dist(rng);

            powerup = powerup_factory.create(Settings::VIRTUAL_WIDTH, y);
        }
    }

    background_x += -Settings::BACK_SCROLL_SPEED * dt;

    if (background_x <= -Settings::BACKGROUND_LOOPING_POINT)
    {
        background_x = 0;
    }

    background.setPosition(background_x, 0);

    ground_x += -Settings::MAIN_SCROLL_SPEED * dt;

    if (ground_x <= -Settings::VIRTUAL_WIDTH)
    {
        ground_x = 0;
    }

    ground.setPosition(ground_x, Settings::VIRTUAL_HEIGHT - Settings::GROUND_HEIGHT);

    for (auto it = logs.begin(); it != logs.end(); )
    {
        if ((*it)->is_out_of_game())
        {
            auto log_pair = *it;
            log_factory.remove(log_pair);
            it = logs.erase(it);
            
        }
        else
        {
            (*it)->update(dt);
            ++it;
        }
    }

    if (powerup) {
        if (powerup->is_out_of_game()) {
            powerup_factory.remove(powerup);
            powerup.reset();
        }
        else {
            powerup->update(dt);
        }
    }
}

void World::render(sf::RenderTarget& target) const noexcept
{
    target.draw(background);

    for (const auto& log_pair: logs)
    {
        log_pair->render(target);
    }

    target.draw(ground);

    if (powerup) {
        powerup->render(target);
    }
    
}