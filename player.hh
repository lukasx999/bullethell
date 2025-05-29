#pragma once

#include <print>
#include <raylib.h>
#include <raymath.h>

#include "particle.hh"

enum class Direction {
    Left,
    Right,
    Up,
    Down,
};

class Player {
    Vector2 m_position;
    float m_radius;
    int m_health;
    const Vector2 m_start_position;
    const int m_max_health;
    const Rectangle m_screen;

public:
    static constexpr int m_healtbar_width = 500;
    static constexpr int m_healtbar_height = 30;

    Player(Vector2 position, float radius, int max_health, Rectangle screen)
        : m_position(position)
        , m_radius(radius)
        , m_health(max_health)
        , m_start_position(position)
        , m_max_health(max_health)
        , m_screen(screen)
    { }

    void reset() {
        m_health = m_max_health;
        m_position = m_start_position;
    }

    [[nodiscard]] bool is_alive() const {
        return m_health > 0;
    }

    void draw_healthbar(Vector2 center, Color fg, Color bg) {
        int w = static_cast<float>(m_health) / m_max_health * m_healtbar_width;
        int x = center.x - m_healtbar_width / 2.0;
        DrawRectangle(x, center.y, w, m_healtbar_height, fg);
        DrawRectangle(x+w, center.y, m_healtbar_width-w, m_healtbar_height, bg);
    }

    void draw() {
        DrawCircleV(m_position, m_radius, BLUE);
    }

    void move(Direction dir) {
        float step = 5;
        // TODO: clamp player position to screen bounds

        switch (dir) {
            case Direction::Left: {
                m_position.x -= step;
            } break;
            case Direction::Right: {
                m_position.x += step;
            } break;
            case Direction::Up: {
                m_position.y -= step;
            } break;
            case Direction::Down: {
                m_position.y += step;
            } break;
        }
    }

    void check_collision(Particle &particle) {
        if (CheckCollisionCircles(m_position, m_radius, particle.position(), particle.radius())) {
            m_health -= 1;
        }
    }

    [[nodiscard]] int health() const {
        return m_health;
    }

};
