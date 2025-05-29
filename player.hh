#pragma once

#include <raylib.h>

#include "projectile.hh"

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
    static constexpr int m_heal_amount = 50;
    static constexpr int m_damage_amount = 0; // TODO: change me back to 1

public:
    static constexpr int m_healtbar_width = 500;
    static constexpr int m_healtbar_height = 30;

    Player(Vector2 position, float radius, int max_health, Rectangle screen);
    void reset();
    void draw_healthbar(Vector2 center, Color fg, Color bg);
    void draw();
    void move(Direction dir);
    void damage();
    void heal();
    [[nodiscard]] bool is_alive() const;
    [[nodiscard]] bool check_collision(const Projectile &particle) const;
    [[nodiscard]] int health() const;

};
