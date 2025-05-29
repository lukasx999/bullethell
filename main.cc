#include <print>
#include <random>
#include <list>
#include <vector>

#include <raylib.h>
#include <raymath.h>

#include "player.hh"
#include "projectile.hh"
#include "util.hh"


class AsyncTimer {
    double m_delay_secs;
    double m_time_threshold = 0;
public:

    AsyncTimer(double delay_secs)
    : m_delay_secs(delay_secs)
    { }

    bool poll() {
        double time = GetTime();
        bool ellapsed = time >= m_time_threshold;

        if (ellapsed)
            m_time_threshold = time + m_delay_secs;

        return ellapsed;
    }

};

class Game {
    Player m_player;
    // Using std::list for storing the projectiles as out-of-bounds projectiles
    // will have to be removed frequently. A queue is not sufficient, as
    // projectiles have different speeds, and ones queued up early, might live longer
    // than ones queued up at a later time.
    std::list<Projectile> m_projectiles;
    AsyncTimer m_timer;
    static constexpr Rectangle m_screen = { 0, 0, 1600, 900 };

    enum class State {
        Running,
        Paused,
        Dead,
        Welcome,
    } m_state = State::Welcome;

public:

    Game()
        : m_player({ m_screen.width/4.0, m_screen.height/4.0 }, 20, 500, m_screen)
        , m_timer(0.0001)
    {
        SetTraceLogLevel(LOG_ERROR);
        SetTargetFPS(60);
        InitWindow(m_screen.width, m_screen.height, "bullethell");
    }

    ~Game() {
        CloseWindow();
    }

    void handle_input_welcome() {
        if (IsKeyPressed(KEY_SPACE))
            m_state = State::Running;
    }

    void draw_welcome() {
        DrawText("Welcome!", 0, 0, 50, RED);
        handle_input_welcome();
    }

    void handle_input_dead() {
        if (IsKeyPressed(KEY_SPACE)) {
            m_state = State::Welcome;
            m_player.reset();
        }
    }

    void draw_dead() {
        DrawText("You Died!", 0, 0, 50, RED);
        handle_input_dead();
    }

    void handle_input_paused() {
        if (IsKeyPressed(KEY_SPACE))
            m_state = State::Running;
    }

    void draw_paused() {
        DrawText("Paused.", 0, 0, 50, RED);
        handle_input_paused();
    }

    void handle_input_running() {
        if (IsKeyDown(KEY_J) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
            m_player.move(Direction::Down);

        if (IsKeyDown(KEY_K) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
            m_player.move(Direction::Up);

        if (IsKeyDown(KEY_H) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
            m_player.move(Direction::Left);

        if (IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            m_player.move(Direction::Right);

        if (IsKeyPressed(KEY_P))
            m_state = State::Paused;
    }

    void draw_ui() {
        DrawFPS(0, 0);
        auto str_health = std::format("Health: {}", m_player.health());
        DrawText(str_health.c_str(), 0, 50, 50, WHITE);

        auto str_particles = std::format("Particles: {}", m_projectiles.size());
        DrawText(str_particles.c_str(), 0, 100, 50, WHITE);

        m_player.draw_healthbar({ m_screen.width/2.0, 0.0 }, RED, GRAY);
    }

    void draw_running() {

        for (auto p=m_projectiles.begin(); p != m_projectiles.end();) {

            if (m_player.check_collision(*p)) {
                switch (p->m_type) {
                    case ProjectileType::Hostile:
                        m_player.damage();
                        break;
                    case ProjectileType::Health:
                        m_player.heal();
                        break;
                }
            }

            p->draw();
            p->update();

            if (!p->is_live()) {
                p = m_projectiles.erase(p);

            } else {
                p++;
            }

        }

        m_player.draw();
        draw_ui();

    }

    void state_running() {

        if (m_timer.poll()) {

            int random = random_range(1, 15);

            Projectile p(
                { m_screen.width/2.0, m_screen.height/2.0 },
                m_screen,
                random == 1
                ? ProjectileType::Health
                : ProjectileType::Hostile
            );

            m_projectiles.push_back(p);
        }

        if (!m_player.is_alive()) {
            m_state = State::Dead;
        }

        draw_running();
        handle_input_running();
    }

    void draw() {
        switch (m_state) {
            case State::Running:
                state_running();
                break;

            case State::Paused:
                draw_paused();
                break;

            case State::Dead:
                draw_dead();
                break;

            case State::Welcome:
                draw_welcome();
                break;
        }
    }

    void loop() {

        while (!WindowShouldClose()) {
            BeginDrawing();
            {
                ClearBackground(BLACK);
                draw();
            }
            EndDrawing();
        }
    }

};



// TODO:
/*
    projectile kinds:
    - hostile
    - healing
    - parryable

    projectile sources:
    - spreading
    - homing
*/

int main() {

    Game game;
    game.loop();

    return 0;
}
