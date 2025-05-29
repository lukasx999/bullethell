#include <print>
#include <random>
#include <vector>

#include <raylib.h>
#include <raymath.h>

#include "player.hh"
#include "particle.hh"



class Game {
    Player m_player;
    std::vector<Particle> m_particles;
    static constexpr int m_width = 1600;
    static constexpr int m_height = 900;
    double m_time_threshold = 0;
    double m_delay_secs = 0.01;
public:

    Game()
    : m_player({ m_width/4.0, m_height/4.0 }, 20, 500)
    {
        SetTargetFPS(60);
        InitWindow(m_width, m_height, "bullethell");
    }

    ~Game() {
        CloseWindow();
    }

    void handle_input() {
        if (IsKeyDown(KEY_J) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
            m_player.move(Direction::Down);
        }

        if (IsKeyDown(KEY_K) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            m_player.move(Direction::Up);
        }

        if (IsKeyDown(KEY_H) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            m_player.move(Direction::Left);
        }

        if (IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            m_player.move(Direction::Right);
        }
    }

    void draw() {

        ClearBackground(BLACK);
        auto str = std::format("Health: {}", m_player.health());
        DrawText(str.c_str(), 0, 0, 50, WHITE);

        if (GetTime() >= m_time_threshold) {

            Particle particle({ m_width/2.0, m_height/2.0 });
            m_particles.push_back(particle);

            m_time_threshold = GetTime() + m_delay_secs;
        }

        for (auto &particle : m_particles) {
            m_player.check_collision(particle);
            particle.draw();
            particle.update();
        }

        if (!m_player.is_alive()) {
            std::println("u dead.");
            exit(1);
        }

        m_player.draw();
        m_player.draw_healthbar({ m_width/2.0, 0.0 }, RED, GRAY);

        handle_input();

    }

    void loop() {

        while (!WindowShouldClose()) {
            BeginDrawing();
            draw();
            EndDrawing();
        }
    }

};



int main() {

    Game game;
    game.loop();

    return 0;
}
