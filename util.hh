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
