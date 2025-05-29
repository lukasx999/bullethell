#pragma once

#include <raylib.h>
#include <raymath.h>

#include "game.hh"

class GamePaused : public IGameState {
    GameState &m_state;

public:
    GamePaused(GameState &state)
    : m_state(state)
    { }

    void update() override {
        draw();
        handle_input();
    }

private:
    void draw() {
        DrawText("Paused.", 0, 0, 50, RED);
    }

    void handle_input() {
        if (IsKeyPressed(KEY_SPACE))
            m_state = GameState::Running;
    }

};
