#pragma once

#include <raylib.h>
#include <raymath.h>

#include "game.hh"

class GameWelcome : public IGameState {
    GameState &m_state;

public:
    GameWelcome(GameState &state) : m_state(state) { }

    void update() override {
        draw();
        handle_input();
    }

private:
    void draw() {
        DrawText("Welcome!", 0, 0, 50, RED);
    }

    void handle_input() {
        if (IsKeyPressed(KEY_SPACE))
            m_state = GameState::Running;
    }

};
