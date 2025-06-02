#include <print>
#include <format>

#include <raylib-cpp.hpp>

#include "running.hh"

GameRunning::GameRunning(GameState &state, const raylib::Rectangle &screen)
    : m_screen(screen)
    , m_player(
        raylib::Vector2(m_screen.width/4.0f, m_screen.height/4.0f),
        20,
        500,
        m_projectile_texture,
        m_screen
    )
    , m_interval(0.001)
    , m_state(state)
{
    m_projectile_texture.Load("./assets/circle6a.png");
    m_projectile_texture.GenMipmaps();
}

void GameRunning::update() {

    if (m_interval.poll())
        spawn_projectile();

    if (!m_player.is_alive()) {
        m_state = GameState::Dead;
        m_player.reset();
    }

    draw();
    handle_input();
}

void GameRunning::spawn_projectile() {
    int random = random_range(1, 15);

    raylib::Vector2 proj_pos(m_screen.width/2.0f, m_screen.height/2.0f);

    // // SPREADING
    // Projectile proj(
    //     raylib::Vector2(m_screen.width/2.0f, m_screen.height/2.0f),
    //     raylib::Vector2(random_range(-5.0f, 5.0f), random_range(-5.0f, 5.0f)),
    //     random == 1
    //         ? ProjectileType::Health
    //         : ProjectileType::Hostile,
    //     m_projectile_texture,
    //     m_screen,
    //     random_range(10, 30)
    // );

    // // HOMING
    // auto proj_speed = 0.025f;
    // Projectile proj(
    //     proj_pos,
    //     raylib::Vector2((m_player.m_position - proj_pos)*proj_speed),
    //     random == 1
    //         ? ProjectileType::Health
    //         : ProjectileType::Hostile,
    //     m_projectile_texture,
    //     m_screen,
    //     random_range(10, 30)
    // );

    auto vel = raylib::Vector2::One()
        .Rotate(Lerp(0, 360, GetTime()/100));

    // CYCLING
    Projectile proj(
        proj_pos,
        vel,
        random == 1
            ? ProjectileType::Health
            : ProjectileType::Hostile,
        m_projectile_texture,
        m_screen,
        random_range(10, 30)
    );





    m_projectiles.push_back(proj);
}

void GameRunning::draw() {

    for (auto p=m_projectiles.begin(); p != m_projectiles.end();) {

        p->update();

        if (m_player.check_collision(*p) && p->is_alive()) {
            switch (p->m_type) {
                case ProjectileType::Hostile:
                    m_player.damage();
                    break;

                case ProjectileType::Health:
                    m_player.heal();
                    break;

                case ProjectileType::Bullet:
                    ;
                    break;
            }
            p->destroy();
        }

        if (p->is_dead())
            p = m_projectiles.erase(p);
        else
            p++;

    }

    m_player.update();
    draw_ui();

    raylib::Mouse::GetPosition().DrawCircle(m_cursor_size, RED);

}

void GameRunning::draw_ui() {
    DrawFPS(0, 0);
    auto str_health = std::format("Health: {}", m_player.health());
    DrawText(str_health.c_str(), 0, 50, 50, WHITE);

    auto str_particles = std::format("Particles: {}", m_projectiles.size());
    DrawText(str_particles.c_str(), 0, 100, 50, WHITE);

    m_player.draw_healthbar(
        { m_screen.width/2.0f, 0.0 },
        RED,
        GRAY
    );
}

void GameRunning::handle_input() {
    if (IsKeyDown(KEY_J) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        m_player.move(Direction::Down);

    if (IsKeyDown(KEY_K) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        m_player.move(Direction::Up);

    if (IsKeyDown(KEY_H) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        m_player.move(Direction::Left);

    if (IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        m_player.move(Direction::Right);

    if (IsKeyPressed(KEY_P))
        m_state = GameState::Paused;
}
