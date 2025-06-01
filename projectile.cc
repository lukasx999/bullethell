#include <print>

#include <raylib-cpp.hpp>

#include "projectile.hh"
#include "util.hh"


Projectile::Projectile(
    raylib::Vector2 origin,
    raylib::Vector2 velocity,
    ProjectileType type,
    const raylib::Texture2D &texture,
    const raylib::Rectangle &screen,
    float radius
)
    : m_screen(screen)
    , m_velocity(velocity)
    , m_texture(texture)
    , m_position(origin)
    , m_radius(radius)
    , m_type(type)
{ }

void Projectile::draw_live() {
    raylib::Color color = m_state_map.at(m_type);

    // draw the texture onto a circle, such that the edges of the texture
    // dont cover any over projectiles on the screen
    m_position.DrawCircle(m_radius, BLACK);
    BeginBlendMode(BLEND_ADDITIVE);
    {
        raylib::Rectangle src    = { 0.0, 0.0, static_cast<float>(m_texture.width), static_cast<float>(m_texture.height) };
        raylib::Rectangle dest   = { m_position.x, m_position.y, m_radius * 2.0f, m_radius * 2.0f };
        raylib::Vector2   origin = { m_radius, m_radius };
        m_texture.Draw(src, dest, origin, 0, color);
    }
    EndBlendMode();
}

void Projectile::draw_fading() {
    Color color = m_state_map.at(m_type);

    float radius = Lerp(
        m_radius,
        m_radius * m_fade_radius_multiplier,
        m_timer.time()
    );
    color.a = Lerp(color.a, 0, m_timer.time());

    m_position.DrawCircle(radius, color);
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

            raylib::Rectangle screen = {
                0.0,
                0.0,
                m_screen.width,
                m_screen.height,
            };

            if (!m_position.CheckCollisionCircle(m_radius, screen))
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
