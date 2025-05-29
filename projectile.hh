#pragma once

#include <print>

#include <raylib.h>
#include <raymath.h>

#include "util.hh"

enum class ProjectileType {
    Hostile,
    Health,
};

class Projectile {
    Vector2 m_velocity;
    bool m_is_live = true;
    const Rectangle m_screen;

public:
    Vector2 m_position;
    float m_radius;
    const ProjectileType m_type;

    Projectile(Vector2 origin, Rectangle screen, ProjectileType type)
        : m_velocity(random_range(-5, 5), random_range(-5, 5))
        , m_screen(screen)
        , m_position(origin)
        , m_radius(random_range(10, 30))
        , m_type(type)
    { }

    [[nodiscard]] bool is_live() const {
        return m_is_live;
    }

    void update() {
        m_is_live = CheckCollisionCircleRec(m_position, m_radius, m_screen);
        m_position.x += m_velocity.x;
        m_position.y += m_velocity.y;
    }

    void draw() {
        Color color;
        switch (m_type) {
            case ProjectileType::Health:
                color = GREEN;
                break;
            case ProjectileType::Hostile:
                color = RED;
                break;
        }
        DrawCircleV(m_position, m_radius, color);
    }

};
