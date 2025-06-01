#pragma once

#include <raylib-cpp.hpp>

#include "game.hh"



class GamePaused : public IGameState {
    GameState &m_state;

public:
    GamePaused(GameState &state);
    void update() override;

private:
    void draw();
    void handle_input();

};



class GameWelcome : public IGameState {
    GameState &m_state;

public:
    GameWelcome(GameState &state);
    void update() override;

private:
    void draw();
    void handle_input();

};



class GameOver : public IGameState {
    GameState &m_state;

public:
    GameOver(GameState &state);
    void update() override;

private:
    void handle_input();

};
