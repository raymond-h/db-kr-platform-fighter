#ifndef __RESET_COLLISION_FLAGS_HPP__
#define __RESET_COLLISION_FLAGS_HPP__

#include <entt/entt.hpp>

#include "components.hpp"

inline void resetCollisionFlags(entt::registry &registry)
{
    registry.view<GroundCollisionFlags>().each([](GroundCollisionFlags &groundCollisionFlags) {
        groundCollisionFlags.left = groundCollisionFlags.top =
            groundCollisionFlags.right = groundCollisionFlags.bottom = false;
    });
}

#endif // __RESET_COLLISION_FLAGS_HPP__
