#ifndef __UPDATE_HITBOX_POSITION_HPP__
#define __UPDATE_HITBOX_POSITION_HPP__

#include <entt/entt.hpp>

#include "components.hpp"

inline void updateHitboxPosition(entt::registry &registry)
{
    registry.view<Position, Hitbox>().each([&](Position &pos, Hitbox &hitbox) {
        auto ownerPos = registry.get<Position>(hitbox.owner);

        pos.x = ownerPos.x + hitbox.ownerOffsetX;
        pos.y = ownerPos.y + hitbox.ownerOffsetY;
    });
}

#endif // __UPDATE_HITBOX_POSITION_HPP__
