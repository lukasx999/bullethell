#include <print>

#include <raylib.h>
#include <raymath.h>

#include "projectile.hh"
#include "util.hh"

Projectile::Projectile(Vector2 origin, ProjectileType type, const Rectangle &screen)
    : m_screen(screen)
    , m_velocity(random_range(-5, 5), random_range(-5, 5))
    , m_position(origin)
    , m_radius(random_range(10, 30))
    , m_type(type)
{ }

void Projectile::draw_live() {
    Color color = m_state_map.at(m_type);
    DrawCircleV(m_position, m_radius, color);
}

void Projectile::draw_fading() {
    Color color = m_state_map.at(m_type);

    float radius = Lerp(
        m_radius,
        m_radius * m_fade_radius_multiplier,
        m_timer.time()
    );
    color.a = Lerp(color.a, 0, m_timer.time());

    DrawCircleV(m_position, radius, color);
}

void Projectile::update() {
    m_position.x += m_velocity.x;
    m_position.y += m_velocity.y;

    switch (m_state) {
        case ProjectileState::Dead:
            ;
            break;

        case ProjectileState::Live: {
            draw_live();

            Rectangle screen = {
                0.0,
                0.0,
                static_cast<float>(m_screen.width),
                static_cast<float>(m_screen.height),
            };
            if (!CheckCollisionCircleRec(m_position, m_radius, screen))
                m_state = ProjectileState::Dead;

        } break;

        case ProjectileState::Fading: {
            draw_fading();

            m_timer.count();

            if (m_timer.has_overflowed())
                m_state = ProjectileState::Dead;
        } break;

    }

}

[[nodiscard]] bool Projectile::is_dead() const {
    return m_state == ProjectileState::Dead;
}

[[nodiscard]] bool Projectile::is_alive() const {
    return m_state == ProjectileState::Live;
}

void Projectile::destroy() {
    // condition prevents bugs, where destroy() revives dead projectiles
    if (m_state == ProjectileState::Live)
        m_state = ProjectileState::Fading;
}
