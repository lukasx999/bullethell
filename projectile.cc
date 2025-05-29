#include <raylib.h>
#include <raymath.h>

#include "projectile.hh"
#include "util.hh"

Projectile::Projectile(Vector2 origin, Rectangle screen, ProjectileType type)
    : m_velocity(random_range(-5, 5), random_range(-5, 5))
    , m_screen(screen)
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

    // BUG: projectiles getting to big when being hovered over
    // for too long by player
    float radius = Lerp(
        m_radius,
        m_radius * m_fade_radius_multiplier,
        m_fade_timer
    );
    color.a = Lerp(color.a, 0, m_fade_timer);

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

            if (!CheckCollisionCircleRec(m_position, m_radius, m_screen))
                m_state = ProjectileState::Dead;

        } break;

        case ProjectileState::Fading: {
            draw_fading();

            m_fade_timer += m_fade_step;

            if (m_fade_timer >= m_fade_max)
                m_state = ProjectileState::Dead;
        } break;

    }

}

[[nodiscard]] bool Projectile::is_alive() const {
    return m_state != ProjectileState::Dead;
}

void Projectile::destroy() {
    m_state = ProjectileState::Fading;
}
