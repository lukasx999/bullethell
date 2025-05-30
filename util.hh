#pragma once

#include <random>

#include <raylib.h>


template <typename T>
static T random_range(T min, T max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

class Interval {
    double m_delay_secs;
    double m_time_threshold = 0;

public:
    Interval(double delay_secs);
    bool poll();

};

class Timer {
    float m_timer = 0.0;
    const float m_top;
    const float m_step;

public:
    Timer(float top, float step);
    [[nodiscard]] float time() const;
    [[nodiscard]] bool has_overflowed() const;
    void reset();
    void count();

};
