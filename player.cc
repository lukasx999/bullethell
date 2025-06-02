#include <print>
#include <raylib-cpp.hpp>

#include "player.hh"



Player::Player(
    raylib::Vector2 position,
    float radius,
    int max_health,
    const raylib::Texture2D &projectile_texture,
    const raylib::Rectangle &screen
)
    : m_screen(screen)
    , m_direction(raylib::Vector2(1.0f, 0.0f))
    , m_radius(radius)
    , m_health(max_health)
    , m_projectile_texture(projectile_texture)
    , m_start_position(position)
    , m_max_health(max_health)
    , m_interval(0.1f)
    , m_position(position)
{ }

void Player::reset() {
    m_health = m_max_health;
    m_position = m_start_position;
}

void Player::draw_healthbar(raylib::Vector2 center, raylib::Color fg, raylib::Color bg)
{
    int w = static_cast<float>(m_health) / m_max_health * m_healtbar_width;
    int x = center.x - m_healtbar_width / 2.0;
    DrawRectangle(x, center.y, w, m_healtbar_height, fg);
    DrawRectangle(x+w, center.y, m_healtbar_width-w, m_healtbar_height, bg);
}

void Player::draw() {
    m_position.DrawCircle(m_radius, BLUE);
    m_position.DrawLine(m_position + m_direction, 5.0f, PURPLE);

    for (auto p=m_projectiles.begin(); p != m_projectiles.end();) {

        p->update();

        if (p->is_dead())
            p = m_projectiles.erase(p);
        else
            p++;

    }

}

void Player::handle_input() {
    m_direction = raylib::Mouse::GetPosition() - m_position;
}

void Player::update() {

    if (m_interval.poll()) {

        Projectile proj(
            { m_position.x, m_position.y },
            m_direction.Normalize() * m_projectile_speed,
            ProjectileType::Bullet,
            m_projectile_texture,
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

    // m_position = m_position.Clamp(
    //     raylib::Vector2::Zero(),
    //     { m_screen.width, m_screen.height }
    // );

    m_position.x = std::clamp(m_position.x, 0.0f, m_screen.width);
    m_position.y = std::clamp(m_position.y, 0.0f, m_screen.height);

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
    return m_position.CheckCollisionCircle(m_radius, particle.m_position, particle.m_radius);
}

[[nodiscard]] int Player::health() const {
    return m_health;
}
