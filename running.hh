#pragma once

#include <list>

#include <raylib-cpp.hpp>

#include "player.hh"
#include "projectile.hh"
#include "game.hh"
#include "util.hh"

class GameRunning : public IGameState {
    const raylib::Rectangle &m_screen;
    Player m_player;
    raylib::Texture2D m_projectile_texture;
    // Using std::list for storing the projectiles as out-of-bounds projectiles
    // will have to be removed frequently. A queue is not sufficient, as
    // projectiles have different speeds, and ones queued up early, might live longer
    // than ones queued up at a later time.
    std::list<Projectile> m_projectiles;
    Interval m_interval;
    GameState &m_state;

public:
    GameRunning(GameState &state, const raylib::Rectangle &screen);
    void update() override;

private:
    void spawn_projectile();
    void draw();
    void draw_ui();
    void handle_input();

};
