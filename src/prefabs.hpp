#ifndef __PREFABS_HPP__
#define __PREFABS_HPP__

#include "components.hpp"

inline auto createPlayer(entt::registry &registry, coord_t x, coord_t y, coord_t w, coord_t h)
{
    auto playerEntity = registry.create();
    registry.emplace<PlayerControllable>(playerEntity);
    registry.emplace<FighterState>(playerEntity, FighterStateEnum::Idle, 0);
    registry.emplace<Position>(playerEntity, x, y);
    registry.emplace<CollisionBox>(playerEntity, w, h);
    registry.emplace<Velocity>(playerEntity, 0, 0);
    registry.emplace<GroundCollisionFlags>(playerEntity, false, false, false, false);
    return playerEntity;
}

inline auto createGround(entt::registry &registry, coord_t x, coord_t y, coord_t w, coord_t h)
{
    auto ent = registry.create();
    registry.emplace<Position>(ent, x, y);
    registry.emplace<CollisionBox>(ent, w, h);
    registry.emplace<Ground>(ent);
    return ent;
}

#endif // __PREFABS_HPP__
