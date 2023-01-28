/*
    ISPPJ1 2023
    Study Case: Flappy Bird

    Author: Alejandro Mujica
    alejandro.j.mujic4@gmail.com

    This file contains the definition of the Settings methods to load multimedia files.
*/

#include <stdexcept>

#include <Settings.hpp>

const fs::path Settings::GRAPHICS_PATH{"graphics"};

const fs::path Settings::SOUNDS_PATH{"sounds"};

const fs::path Settings::FONTS_PATH{"fonts"};

std::unordered_map<std::string, sf::Texture> Settings::textures{};

std::unordered_map<std::string, sf::SoundBuffer> Settings::sound_buffers;

std::unordered_map<std::string, sf::Sound> Settings::sounds{};

std::unordered_map<std::string, sf::Font> Settings::fonts{};

sf::Music Settings::music{};
sf::Music Settings::powerup_music{};

void Settings::init()
{
    Settings::load_textures();
    Settings::load_sounds();
    Settings::load_fonts();
}

void Settings::load_textures()
{
    sf::Texture texture{};

    if (!texture.loadFromFile(Settings::GRAPHICS_PATH / "bird.png"))
    {
        throw std::runtime_error{"Error loading texture graphics/bird.png"};
    }

    Settings::textures["bird"] = texture;

    if (!texture.loadFromFile(Settings::GRAPHICS_PATH / "background.png"))
    {
        throw std::runtime_error{"Error loading texture graphics/background.png"};
    }

    Settings::textures["background"] = texture;

    if (!texture.loadFromFile(Settings::GRAPHICS_PATH / "ground.png"))
    {
        throw std::runtime_error{"Error loading texture graphics/ground.png"};
    }

    Settings::textures["ground"] = texture;

    if (!texture.loadFromFile(Settings::GRAPHICS_PATH / "log.png"))
    {
        throw std::runtime_error{"Error loading texture graphics/log.png"};
    }

    Settings::textures["Log"] = texture;

    if (!texture.loadFromFile(Settings::GRAPHICS_PATH / "feather.png"))
    {
        throw std::runtime_error{"Error loading texture graphics/feather.png"};
    }

    Settings::textures["feather"] = texture;

     if (!texture.loadFromFile(Settings::GRAPHICS_PATH / "bird_ghost.png"))
    {
        throw std::runtime_error{"Error loading texture graphics/bird_ghost.png"};
    }

    Settings::textures["bird_ghost"] = texture;
    
}

void Settings::load_sounds()
{
    sf::SoundBuffer buffer;
    sf::Sound sound;
    
    if (!buffer.loadFromFile(Settings::SOUNDS_PATH / "jump.wav"))
    {
        throw std::runtime_error{"Error loading sound sounds/jump.wav"};
    }

    auto result = Settings::sound_buffers.emplace("jump", buffer);

    sound.setBuffer(result.first->second);
    Settings::sounds["jump"] = sound;

    if (!buffer.loadFromFile(Settings::SOUNDS_PATH / "explosion.wav"))
    {
        throw std::runtime_error{"Error loading sound sounds/explosion.wav"};
    }

    result = Settings::sound_buffers.emplace("explosion", buffer);

    sound.setBuffer(result.first->second);
    Settings::sounds["explosion"] = sound;

    if (!buffer.loadFromFile(Settings::SOUNDS_PATH / "hurt.wav"))
    {
        throw std::runtime_error{"Error loading sound sounds/hurt.wav"};
    }

    result = Settings::sound_buffers.emplace("hurt", buffer);

    sound.setBuffer(result.first->second);
    Settings::sounds["hurt"] = sound;

    if (!buffer.loadFromFile(Settings::SOUNDS_PATH / "score.wav"))
    {
        throw std::runtime_error{"Error loading sound sounds/score.wav"};
    }

    result = Settings::sound_buffers.emplace("score", buffer);

    sound.setBuffer(result.first->second);
    Settings::sounds["score"] = sound;

    if (!Settings::music.openFromFile(Settings::SOUNDS_PATH / "marios_way.ogg"))
    {
        throw std::runtime_error{"Error loading music sounds/marios_way.ogg"};
    }

    if (!buffer.loadFromFile(Settings::SOUNDS_PATH / "hit_someting.wav"))
    {
        throw std::runtime_error{"Error loading sound sounds/hit_someting.wav"};
    }

    result = Settings::sound_buffers.emplace("hit_someting", buffer);

    sound.setBuffer(result.first->second);
    Settings::sounds["hit_someting"] = sound;

    if (!buffer.loadFromFile(Settings::SOUNDS_PATH / "leave_powerup.wav"))
    {
        throw std::runtime_error{"Error loading sound sounds/leave_powerup.wav"};
    }

    result = Settings::sound_buffers.emplace("leave_powerup", buffer);

    sound.setBuffer(result.first->second);
    Settings::sounds["leave_powerup"] = sound;

    if (!buffer.loadFromFile(Settings::SOUNDS_PATH / "get_powerup.wav"))
    {
        throw std::runtime_error{"Error loading sound sounds/get_powerup.wav"};
    }

    result = Settings::sound_buffers.emplace("get_powerup", buffer);

    sound.setBuffer(result.first->second);
    Settings::sounds["get_powerup"] = sound;

    if (!Settings::powerup_music.openFromFile(Settings::SOUNDS_PATH / "powerup_music.ogg"))
    {
        throw std::runtime_error{"Error loading music sounds/powerup_music.ogg"};
    }
}

void Settings::load_fonts()
{
    sf::Font font;

    if (!font.loadFromFile(Settings::FONTS_PATH / "font.ttf"))
    {
        throw std::runtime_error{"Error loading font fonts/font.ttf"};
    }

    Settings::fonts["font"] = font;

    if (!font.loadFromFile(Settings::FONTS_PATH / "flappy.ttf"))
    {
        throw std::runtime_error{"Error loading font fonts/flappy.ttf"};
    }

    Settings::fonts["flappy"] = font;
}
