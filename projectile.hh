#pragma once

#include <unordered_map>

#include <raylib.h>

enum class ProjectileType {
    Hostile,
    Health,
};

class Projectile {
    Vector2 m_velocity;
    float m_fade_timer = 0.0;
    const Rectangle m_screen;
    static constexpr float m_fade_max = 1.0;
    static constexpr float m_fade_step = 0.1;
    static constexpr float m_fade_radius_multiplier = 1.5;
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

    Projectile(Vector2 origin, Rectangle screen, ProjectileType type);
    void update();
    [[nodiscard]] bool is_alive() const;
    void destroy();

private:
    void draw_live();
    void draw_fading();

};
