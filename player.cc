#include <print>
#include <raylib.h>
#include <raymath.h>

#include "player.hh"

Player::Player(Vector2 position, float radius, int max_health, const Rectangle &screen)
    : m_screen(screen)
    , m_position(position)
    , m_direction({ 1.0f, 0.0f })
    , m_radius(radius)
    , m_health(max_health)
    , m_start_position(position)
    , m_max_health(max_health)
    , m_interval(0.1f)
{ }

void Player::reset() {
    m_health = m_max_health;
    m_position = m_start_position;
}

void Player::draw_healthbar(Vector2 center, Color fg, Color bg) {
    int w = static_cast<float>(m_health) / m_max_health * m_healtbar_width;
    int x = center.x - m_healtbar_width / 2.0;
    DrawRectangle(x, center.y, w, m_healtbar_height, fg);
    DrawRectangle(x+w, center.y, m_healtbar_width-w, m_healtbar_height, bg);
}

void Player::draw() {
    DrawCircleV(m_position, m_radius, BLUE);

    DrawLineEx(
        m_position,
        Vector2Add(m_position, m_direction),
        5.0f,
        PURPLE
    );

    for (auto p=m_projectiles.begin(); p != m_projectiles.end();) {

        p->update();

        if (p->is_dead())
            p = m_projectiles.erase(p);
        else
            p++;

    }
}

void Player::handle_input() {
    m_direction = Vector2Subtract(GetMousePosition(), m_position);
}

void Player::update() {

    if (m_interval.poll()) {

        Projectile proj(
            { m_position.x, m_position.y },
            Vector2Normalize(m_direction),
            ProjectileType::Bullet,
            m_screen,
            10.0f
        );

        m_projectiles.push_back(proj);
    }

    handle_input();
    draw();

}

void Player::move(Direction dir) {
    float step = 5;

    switch (dir) {
        case Direction::Left:
            m_position.x -= step;
            break;

        case Direction::Right:
            m_position.x += step;
            break;

        case Direction::Up:
            m_position.y -= step;
            break;

        case Direction::Down:
            m_position.y += step;
            break;
    }

    m_position.x = std::clamp(m_position.x, 0.0f, static_cast<float>(m_screen.width));
    m_position.y = std::clamp(m_position.y, 0.0f, static_cast<float>(m_screen.height));

}

void Player::damage() {
    m_health -= m_damage_amount;
}

void Player::heal() {
    m_health = std::min(m_health+m_heal_amount, m_max_health);
}

[[nodiscard]] bool Player::is_alive() const {
    return m_health > 0;
}

[[nodiscard]] bool Player::check_collision(const Projectile &particle) const {
    return CheckCollisionCircles(m_position, m_radius, particle.m_position, particle.m_radius);
}

[[nodiscard]] int Player::health() const {
    return m_health;
}
