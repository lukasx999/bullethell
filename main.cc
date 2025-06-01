#include <print>
#include <unordered_map>

#include <raylib-cpp.hpp>

#include "game.hh"
#include "running.hh"
#include "states.hh"


class Game {
    const raylib::Rectangle m_screen { 0, 0, 1600, 900 };
    raylib::Window m_window;
    GameRunning m_running;
    GameWelcome m_welcome;
    GamePaused m_paused;
    GameOver m_dead;
    GameState m_state = GameState::Welcome;
    // passing a reference to this to all subclasses so they can adjust to
    // screen dimension changes
    std::unordered_map<GameState, IGameState&> m_state_map {
        { GameState::Running, m_running },
        { GameState::Welcome, m_welcome },
        { GameState::Paused,  m_paused  },
        { GameState::Dead,    m_dead    },
    };

public:
    Game();
    void loop();

private:
    void draw();

};

Game::Game()
    : m_window(m_screen.width, m_screen.height, "bullethell")
    , m_running(m_state, m_screen)
    , m_welcome(m_state)
    , m_paused(m_state)
    , m_dead(m_state)
{
    m_window.SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    m_window.SetTargetFPS(60);
}

void Game::loop() {
    while (!m_window.ShouldClose()) {
        BeginDrawing();
        {
            // m_screen.width = GetScreenWidth();
            // m_screen.height = GetScreenHeight();
            ClearBackground(BLACK);
            draw();
        }
        EndDrawing();
    }
}

void Game::draw() {
    m_state_map.at(m_state).update();
}




// TODO:
/*
    projectile kinds:
    - hostile
    - healing
    - parryable

    projectile sources:
    - spreading
    - homing
    - circling projectiles with varying rotation speed
*/

int main() {

    SetTraceLogLevel(LOG_ERROR);
    Game game;
    game.loop();

    return 0;
}
