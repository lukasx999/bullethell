#pragma once

#include <random>

template <typename T>
static T random_range(T min, T max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}
