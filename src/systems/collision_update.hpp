#ifndef __COLLISION_UPDATE_HPP__
#define __COLLISION_UPDATE_HPP__

#include <entt/entt.hpp>

#include "components.hpp"

inline void handleGroundCollision(entt::registry &registry, entt::entity ground, entt::entity other)
{
    auto [apos, acbox] = registry.get<Position, CollisionBox>(ground);
    auto [bpos, bcbox] = registry.get<Position, CollisionBox>(other);
    auto bGColFlags = registry.try_get<GroundCollisionFlags>(other);

    auto aAabb = boxForEntity(apos, acbox);
    auto bAabb = boxForEntity(bpos, bcbox);

    coord_t overlapX = 0, overlapY = 0;
    calculateOverlap(aAabb, bAabb, overlapX, overlapY);

    if (abs(overlapX) < abs(overlapY))
    {
        bpos.x += overlapX;
        if (overlapX != 0 && bGColFlags != nullptr)
        {
            if (overlapX < 0)
            {
                bGColFlags->right = true;
            }
            else
            {
                bGColFlags->left = true;
            }
        }
    }
    else
    {
        bpos.y += overlapY;
        if (overlapY != 0 && bGColFlags != nullptr)
        {
            if (overlapY < 0)
            {
                bGColFlags->bottom = true;
            }
            else
            {
                bGColFlags->top = true;
            }
        }
    }
}

inline void handleHitboxCollision(entt::registry &registry, entt::entity hitboxEnt, entt::entity other)
{
    auto [apos, acbox, aHitbox] = registry.get<Position, CollisionBox, Hitbox>(hitboxEnt);
    if (aHitbox.owner == other)
        return;

    auto [bpos, bcbox, bVel] = registry.get<Position, CollisionBox, Velocity>(other);

    auto aAabb = boxForEntity(apos, acbox);
    auto bAabb = boxForEntity(bpos, bcbox);

    if (isOverlap(aAabb, bAabb))
    {
        bVel.x = aHitbox.impulseX;
        bVel.y = aHitbox.impulseY;
    }
}

inline void collisionUpdate(entt::registry &registry)
{
    auto ents = registry.view<Position, CollisionBox>();
    auto entsEnd = ents.end();
    for (auto it = ents.begin(); it != entsEnd; it++)
    {
        auto itInner = it;
        itInner++;
        for (; itInner != entsEnd; itInner++)
        {
            auto a = *it;
            auto b = *itInner;

            auto aIsGround = registry.has<Ground>(a);
            auto bIsGround = registry.has<Ground>(b);

            auto aIsHitbox = registry.has<Hitbox>(a);
            auto bIsHitbox = registry.has<Hitbox>(b);

            assert(!(aIsGround && aIsHitbox));
            assert(!(bIsGround && bIsHitbox));

            auto aIsPlain = !(aIsGround || aIsHitbox);
            auto bIsPlain = !(bIsGround || bIsHitbox);

            if (aIsGround && bIsPlain)
            {
                handleGroundCollision(registry, a, b);
            }
            else if (aIsPlain && bIsGround)
            {
                handleGroundCollision(registry, b, a);
            }
            else if (aIsHitbox && bIsPlain)
            {
                handleHitboxCollision(registry, a, b);
            }
            else if (aIsPlain && bIsHitbox)
            {
                handleHitboxCollision(registry, b, a);
            }
        }
    }
}

#endif // __COLLISION_UPDATE_HPP__
