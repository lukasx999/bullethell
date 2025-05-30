#include "util.hh"

#include <raylib.h>

Interval::Interval(double delay_secs)
: m_delay_secs(delay_secs)
{ }

bool Interval::poll() {
    double time = GetTime();
    bool ellapsed = time >= m_time_threshold;

    if (ellapsed)
        m_time_threshold = time + m_delay_secs;

    return ellapsed;
}

Timer::Timer(float top, float step)
    : m_top(top)
    , m_step(step)
{ }

[[nodiscard]] float Timer::time() const {
    return m_timer;
}

[[nodiscard]] bool Timer::has_overflowed() const {
    return m_timer >= m_top;
}

void Timer::reset() {
    m_timer = 0.0;
}

void Timer::count() {
    m_timer += m_step;
}
