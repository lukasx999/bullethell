#pragma once

#include <unordered_map>

#include <raylib-cpp.hpp>

#include "util.hh"

enum class ProjectileType {
    Hostile,
    Health,
    Bullet,
};

class Projectile {
    const raylib::Rectangle &m_screen;
    raylib::Vector2 m_velocity;
    Timer m_timer { 1.0, 0.1 };
    static constexpr float m_fade_radius_multiplier = 1.5;
    raylib::Texture2D m_texture;
    enum class ProjectileState {
        Live,
        Fading,
        Dead,
    } m_state = ProjectileState::Live;
    const std::unordered_map<ProjectileType, raylib::Color> m_state_map {
        { ProjectileType::Health,  GREEN  },
        { ProjectileType::Hostile, RED    },
        { ProjectileType::Bullet,  PURPLE },
    };

public:
    raylib::Vector2 m_position;
    float m_radius;
    const ProjectileType m_type;

    Projectile(
        raylib::Vector2 origin,
        raylib::Vector2 velocity,
        ProjectileType type,
        const raylib::Rectangle &screen,
        float radius
    );
    void update();
    [[nodiscard]] bool is_dead() const;
    [[nodiscard]] bool is_alive() const;
    void destroy();

private:
    void draw_live();
    void draw_fading();

};
