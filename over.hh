#pragma once

#include <raylib.h>
#include <raymath.h>

#include "game.hh"

class GameOver : public IGameState {
    GameState &m_state;

public:
    GameOver(GameState &state)
    : m_state(state)
    { }

    void update() override {
        DrawText("You Died!", 0, 0, 50, RED);
        handle_input();
    }

private:
    void handle_input() {
        if (IsKeyPressed(KEY_SPACE)) {
            m_state = GameState::Welcome;
        }
    }

};
