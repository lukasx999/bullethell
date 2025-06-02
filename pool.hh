#pragma once

#include <list>

#include <raylib-cpp.hpp>

#include "util.hh"
#include "projectile.hh"
#include "player.hh"



class ProjectilePool {
protected:
    const raylib::Rectangle &m_screen;
    const raylib::Texture2D& m_projectile_texture;
    std::list<Projectile> m_projectiles;
    Player &m_player;

public:
    ProjectilePool(
        const raylib::Rectangle &screen,
        const raylib::Texture2D& projectile_texture,
        Player &player
    )
        : m_screen(screen)
        , m_projectile_texture(projectile_texture)
        , m_player(player)
    { }

    void spawn() {
        m_projectiles.push_back(new_projectile());
    }

    void update() {

        for (auto p=m_projectiles.begin(); p != m_projectiles.end();) {

            p->update();

            if (m_player.check_collision(*p) && p->is_alive()) {
                switch (p->m_type) {
                    case ProjectileType::Hostile:
                        m_player.damage();
                        break;

                    case ProjectileType::Health:
                        m_player.heal();
                        break;

                    case ProjectileType::Bullet:
                        ;
                        break;
                }
                p->destroy();
            }

            if (p->is_dead())
                p = m_projectiles.erase(p);
            else
                p++;

        }
    }

protected:
    virtual Projectile new_projectile() = 0;

};



class HomingProjectiles : public ProjectilePool {

public:
    HomingProjectiles(
        const raylib::Rectangle &screen,
        const raylib::Texture2D& projectile_texture,
        Player &player
    )
    : ProjectilePool(screen, projectile_texture, player)
    { }

    Projectile new_projectile() override {

        raylib::Vector2 proj_pos(m_screen.width/2.0f, m_screen.height/2.0f);
        int random = random_range(1, 15);

        float proj_speed = 0.025f;
        auto velocity = raylib::Vector2((m_player.m_position - proj_pos) * proj_speed);

        Projectile proj(
            proj_pos,
            velocity,
            random == 1
            ? ProjectileType::Health
            : ProjectileType::Hostile,
            m_projectile_texture,
            m_screen,
            random_range(10, 30)
        );

        return proj;
    }

};
