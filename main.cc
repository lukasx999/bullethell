#include <print>
#include <random>
#include <list>
#include <vector>

#include <raylib.h>
#include <raymath.h>

#include "player.hh"
#include "particle.hh"


class Timer {
    double m_delay_secs;
    double m_time_threshold = 0;
public:

    Timer(double delay_secs)
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
    // Using std::list for storing the particles as out-of-bounds particles
    // will have to be removed frequently. A queue is not sufficient, as
    // particles have different speeds, and ones queued up early, might live longer
    // than ones queued up at a later time.
    std::list<Particle> m_particles;
    Timer m_timer;
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

    void draw_running() {
        DrawFPS(0, 0);
        auto str_health = std::format("Health: {}", m_player.health());
        DrawText(str_health.c_str(), 0, 50, 50, WHITE);

        auto str_particles = std::format("Particles: {}", m_particles.size());
        DrawText(str_particles.c_str(), 0, 100, 50, WHITE);

        m_player.draw();
        m_player.draw_healthbar({ m_screen.width/2.0, 0.0 }, RED, GRAY);

        for (auto p=m_particles.begin(); p != m_particles.end();) {
            m_player.check_collision(*p);
            p->draw();
            p->update();

            if (!CheckCollisionCircleRec(p->position(), p->radius(), m_screen)) {
                p = m_particles.erase(p);
            } else {
                p++;
            }
        }

    }

    void state_running() {

        if (m_timer.poll()) {
            Particle particle({ m_screen.width/2.0, m_screen.height/2.0 });
            m_particles.push_back(particle);
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
    particle kinds:
    - hostile
    - healing
    - parryable

    particle sources:
    - spreading
    - homing
*/

int main() {

    Game game;
    game.loop();

    return 0;
}
