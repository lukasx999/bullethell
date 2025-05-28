#include <print>
#include <random>
#include <vector>

#include <raylib.h>
#include <raymath.h>

constexpr int WIDTH  = 1600;
constexpr int HEIGHT = 900;

template <typename T>
static T random_range(T min, T max) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}


class Particle {
    Vector2 m_position;
    Vector2 m_velocity;
    float m_radius;

public:

    Particle(Vector2 origin, float radius)
        : m_position(origin)
        , m_velocity(
            random_range(-5, 5),
            random_range(-5, 5)
        )
        , m_radius(radius)
    { }

    [[nodiscard]] Vector2 position() const {
        return m_position;
    }

    [[nodiscard]] float radius() const {
        return m_radius;
    }

    void update() {
        m_position.x += m_velocity.x;
        m_position.y += m_velocity.y;
    }

    void draw() {
        DrawCircleV(m_position, m_radius, RED);
    }

};



enum class Direction {
    Left,
    Right,
    Up,
    Down,
};

class Player {
    Vector2 m_position;
    float m_radius;
    int m_health;
    const int m_max_health;

public:
    const int m_healtbar_width;
    const int m_healtbar_height;

    Player(Vector2 position, float radius, int max_health)
        : m_position(position)
        , m_radius(radius)
        , m_health(max_health)
        , m_max_health(max_health)
        , m_healtbar_width(500)
        , m_healtbar_height(30)
    { }

    void draw_healthbar(Vector2 position_center, Color fg, Color bg) {
        int w = static_cast<float>(m_health) / m_max_health * m_healtbar_width;
        int x = position_center.x - m_healtbar_width/2.0;
        DrawRectangle(x, position_center.y, w, m_healtbar_height, fg);
        DrawRectangle(x+w, position_center.y, m_healtbar_width-w, m_healtbar_height, bg);
    }

    void draw() {
        DrawCircleV(m_position, m_radius, BLUE);
    }

    void move(Direction dir) {
        float step = 5;
        switch (dir) {
            case Direction::Left: {
                m_position.x -= step;
            } break;
            case Direction::Right: {
                m_position.x += step;
            } break;
            case Direction::Up: {
                m_position.y -= step;
            } break;
            case Direction::Down: {
                m_position.y += step;
            } break;
        }
    }

    void check_collision(Particle &particle) {
        if (CheckCollisionCircles(m_position, m_radius, particle.position(), particle.radius())) {
            m_health -= 1;
        }
    }

    [[nodiscard]] int health() const {
        return m_health;
    }

};



int main() {

    Player player({ WIDTH/4.0, HEIGHT/4.0 }, 30, 500);

    std::vector<Particle> particles;


    SetTargetFPS(60);
    InitWindow(WIDTH, HEIGHT, "bullethell");

    double time_threshold = 0;
    double delay_secs = 0.05;

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(BLACK);
            auto str = std::format("Health: {}", player.health());
            DrawText(str.c_str(), 0, 0, 50, WHITE);

            if (GetTime() >= time_threshold) {

                Particle particle({ WIDTH/2.0, HEIGHT/2.0 }, 30);
                particles.push_back(particle);

                time_threshold = GetTime() + delay_secs;
            }

            for (auto &particle : particles) {
                player.check_collision(particle);
                particle.draw();
                particle.update();
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
