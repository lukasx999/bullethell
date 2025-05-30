#pragma once

#include <list>

#include <raylib.h>
#include <raymath.h>

#include "player.hh"
#include "projectile.hh"
#include "game.hh"
#include "util.hh"

class GameRunning : public IGameState {
    const Rectangle &m_screen;
    Player m_player;
    // Using std::list for storing the projectiles as out-of-bounds projectiles
    // will have to be removed frequently. A queue is not sufficient, as
    // projectiles have different speeds, and ones queued up early, might live longer
    // than ones queued up at a later time.
    std::list<Projectile> m_projectiles;
    Interval m_interval;
    GameState &m_state;

public:
    GameRunning(GameState &state, const Rectangle &screen);
    void update() override;

private:
    void spawn_projectile();
    void draw();
    void draw_ui();
    void handle_input();

};
