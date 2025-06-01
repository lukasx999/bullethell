#include "states.hh"

GamePaused::GamePaused(GameState &state)
: m_state(state)
{ }

void GamePaused::update() {
    draw();
    handle_input();
}

void GamePaused::draw() {
    DrawText("Paused.", 0, 0, 50, RED);
}

void GamePaused::handle_input() {
    if (IsKeyPressed(KEY_SPACE))
        m_state = GameState::Running;
}



GameWelcome::GameWelcome(GameState &state)
: m_state(state)
{ }

void GameWelcome::update() {
    draw();
    handle_input();
}

void GameWelcome::draw() {
    DrawText("Welcome!", 0, 0, 50, RED);
}

void GameWelcome::handle_input() {
    if (IsKeyPressed(KEY_SPACE))
        m_state = GameState::Running;
}



GameOver::GameOver(GameState &state)
: m_state(state)
{ }

void GameOver::update() {
    DrawText("You Died!", 0, 0, 50, RED);
    handle_input();
}

void GameOver::handle_input() {
    if (IsKeyPressed(KEY_SPACE)) {
        m_state = GameState::Welcome;
    }
}
