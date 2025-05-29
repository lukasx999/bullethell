#pragma once

#include <print>
#include <random>

#include <raylib.h>
#include <raymath.h>

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

    Particle(Vector2 origin)
        : m_position(origin)
        , m_velocity(
            random_range(-5, 5),
            random_range(-5, 5)
        )
        , m_radius(random_range(10, 30))
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
