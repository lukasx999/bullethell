#include <print>
#include <random>
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
    std::vector<Particle> m_particles;
    Timer m_timer;
    static constexpr int m_width = 1600;
    static constexpr int m_height = 900;

    enum class State {
        Running,
        Paused,
        Dead,
        Welcome,
    } m_state = State::Welcome;

public:

    Game()
        : m_player({ m_width/4.0, m_height/4.0 }, 20, 500)
        , m_timer(0.01)
    {
        SetTargetFPS(60);
        InitWindow(m_width, m_height, "bullethell");
    }

    ~Game() {
        CloseWindow();
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
            m_state = State::Paused;
    }

    void draw_welcome() {
        DrawText("Welcome!", 0, 0, 50, RED);
        if (IsKeyPressed(KEY_SPACE))
            m_state = State::Running;
    }

    void draw_dead() {
        DrawText("You Died!", 0, 0, 50, RED);
        if (IsKeyPressed(KEY_SPACE)) {
            m_state = State::Welcome;
            m_player.reset();
        }
    }

    void draw_paused() {
        DrawText("Paused.", 0, 0, 50, RED);
        if (IsKeyPressed(KEY_SPACE))
            m_state = State::Running;
    }

    void draw_running() {

        auto str = std::format("Health: {}", m_player.health());
        DrawText(str.c_str(), 0, 0, 50, WHITE);

        if (m_timer.poll()) {
            Particle particle({ m_width/2.0, m_height/2.0 });
            m_particles.push_back(particle);
        }

        for (auto &particle : m_particles) {
            m_player.check_collision(particle);
            particle.draw();
            particle.update();
        }

        if (!m_player.is_alive()) {
            m_state = State::Dead;
        }

        m_player.draw();
        m_player.draw_healthbar({ m_width/2.0, 0.0 }, RED, GRAY);

        handle_input();

    }

    void draw() {
        switch (m_state) {
            case State::Running:
                draw_running();
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



int main() {

    Game game;
    game.loop();

    return 0;
}
