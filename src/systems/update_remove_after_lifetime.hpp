#ifndef __UPDATE_REMOVE_AFTER_LIFETIME_HPP__
#define __UPDATE_REMOVE_AFTER_LIFETIME_HPP__

#include <entt/entt.hpp>

#include "components.hpp"

inline void updateRemoveAfterLifetime(entt::registry &registry)
{
    registry.view<RemoveAfterLifetime>().each([&](const auto ent, RemoveAfterLifetime &rAL) {
        if (rAL.lifetime-- <= 0)
        {
            registry.destroy(ent);
        }
    });
}

#endif // __UPDATE_REMOVE_AFTER_LIFETIME_HPP__
