#pragma once

enum class GameState {
    Running,
    Paused,
    Dead,
    Welcome,
};

class IGameState {
public:
    virtual void update() = 0;
};

