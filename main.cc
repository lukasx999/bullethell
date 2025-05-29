#include <print>
#include <random>
#include <vector>

#include <raylib.h>
#include <raymath.h>

#include "player.hh"
#include "particle.hh"

constexpr int WIDTH  = 1600;
constexpr int HEIGHT = 900;


struct Game {
    // TODO:
};



int main() {

    Player player({ WIDTH/4.0, HEIGHT/4.0 }, 20, 500);
    std::vector<Particle> particles;

    SetTargetFPS(60);
    InitWindow(WIDTH, HEIGHT, "bullethell");

    double time_threshold = 0;
    double delay_secs = 0.01;

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(BLACK);
            auto str = std::format("Health: {}", player.health());
            DrawText(str.c_str(), 0, 0, 50, WHITE);

            if (GetTime() >= time_threshold) {

                Particle particle({ WIDTH/2.0, HEIGHT/2.0 });
                particles.push_back(particle);

                time_threshold = GetTime() + delay_secs;
            }

            for (auto &particle : particles) {
                player.check_collision(particle);
                particle.draw();
                particle.update();
            }

            if (!player.is_alive()) {
                std::println("u dead.");
                exit(1);
            }

            player.draw();
            player.draw_healthbar({ WIDTH/2.0, 0.0 }, RED, GRAY);

            if (IsKeyDown(KEY_J) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
                player.move(Direction::Down);
            }

            if (IsKeyDown(KEY_K) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
                player.move(Direction::Up);
            }

            if (IsKeyDown(KEY_H) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                player.move(Direction::Left);
            }

            if (IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                player.move(Direction::Right);
            }

        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
