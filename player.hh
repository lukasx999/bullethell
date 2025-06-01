#pragma once

#include <list>

#include <raylib.h>

#include "projectile.hh"
#include "util.hh"

enum class Direction {
    Left,
    Right,
    Up,
    Down,
};

class Player {
    const Rectangle &m_screen;
    Vector2 m_position;
    Vector2 m_direction;
    float m_radius;
    int m_health;
    const Vector2 m_start_position;
    const int m_max_health;
    static constexpr int m_heal_amount = 50;
    static constexpr int m_damage_amount = 1;
    std::list<Projectile> m_projectiles;
    Interval m_interval;

public:
    static constexpr int m_healtbar_width = 500;
    static constexpr int m_healtbar_height = 30;

    Player(Vector2 position, float radius, int max_health, const Rectangle &screen);
    void reset();
    void draw_healthbar(Vector2 center, Color fg, Color bg);
    void update();
    void move(Direction dir);
    void damage();
    void heal();
    [[nodiscard]] bool is_alive() const;
    [[nodiscard]] bool check_collision(const Projectile &particle) const;
    [[nodiscard]] int health() const;

private:
    void draw();
    void handle_input();

};
