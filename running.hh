#pragma once

#include <list>
#include <format>

#include <raylib.h>
#include <raymath.h>

#include "player.hh"
#include "projectile.hh"
#include "game.hh"
#include "util.hh"

class GameRunning : public IGameState {
    const Rectangle m_screen;
    Player m_player;
    // Using std::list for storing the projectiles as out-of-bounds projectiles
    // will have to be removed frequently. A queue is not sufficient, as
    // projectiles have different speeds, and ones queued up early, might live longer
    // than ones queued up at a later time.
    std::list<Projectile> m_projectiles;
    AsyncTimer m_timer;
    GameState &m_state;

public:
    GameRunning(Rectangle screen, GameState &state)
        : m_screen(screen)
        , m_player(
            { static_cast<float>(m_screen.width/4.0), static_cast<float>(m_screen.height/4.0) },
            20,
            500,
            m_screen
        )
        , m_timer(0.0001)
        , m_state(state)
    { }

    void update() override {

        if (m_timer.poll()) {
            spawn_projectile();
        }

        if (!m_player.is_alive()) {
            m_state = GameState::Dead;
            m_player.reset();
        }

        draw();
        handle_input();
    }

private:
    void spawn_projectile() {
        int random = random_range(1, 15);

        Projectile p(
            { static_cast<float>(m_screen.width/2.0), static_cast<float>(m_screen.height/2.0) },
            m_screen,
            random == 1
            ? ProjectileType::Health
            : ProjectileType::Hostile
        );

        m_projectiles.push_back(p);
    }

    void draw() {

        for (auto p=m_projectiles.begin(); p != m_projectiles.end();) {

            p->update();

            if (m_player.check_collision(*p)) {
                switch (p->m_type) {
                    case ProjectileType::Hostile:
                        m_player.damage();
                        break;

                    case ProjectileType::Health:
                        m_player.heal();
                        break;
                }
                p->destroy();
            }

            if (!p->is_alive())
                p = m_projectiles.erase(p);
            else
                p++;

        }

        m_player.draw();
        draw_ui();

    }

    void draw_ui() {
        DrawFPS(0, 0);
        auto str_health = std::format("Health: {}", m_player.health());
        DrawText(str_health.c_str(), 0, 50, 50, WHITE);

        auto str_particles = std::format("Particles: {}", m_projectiles.size());
        DrawText(str_particles.c_str(), 0, 100, 50, WHITE);

        m_player.draw_healthbar(
            { static_cast<float>(m_screen.width/2.0), 0.0 },
            RED,
            GRAY
        );
    }

    void handle_input() {
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

};
