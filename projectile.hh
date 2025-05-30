#pragma once

#include <unordered_map>

#include <raylib.h>

#include "util.hh"

enum class ProjectileType {
    Hostile,
    Health,
};

class Projectile {
    const Rectangle &m_screen;
    Vector2 m_velocity;
    Timer m_timer { 1.0, 0.1 };
    static constexpr float m_fade_radius_multiplier = 1.5;
    Texture2D m_texture;
    enum class ProjectileState {
        Live,
        Fading,
        Dead,
    } m_state = ProjectileState::Live;
    const std::unordered_map<ProjectileType, Color> m_state_map {
        { ProjectileType::Health,  GREEN },
        { ProjectileType::Hostile, RED   },
    };

public:
    Vector2 m_position;
    float m_radius;
    const ProjectileType m_type;

    Projectile(Vector2 origin, ProjectileType type, const Rectangle &screen);
    void update();
    [[nodiscard]] bool is_dead() const;
    [[nodiscard]] bool is_alive() const;
    void destroy();

private:
    void draw_live();
    void draw_fading();

};
