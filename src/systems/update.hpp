#ifndef __UPDATE_HPP__
#define __UPDATE_HPP__

#include <entt/entt.hpp>

#include "input_update.hpp"
#include "position_update.hpp"
#include "reset_collision_flags.hpp"
#include "collision_update.hpp"
#include "update_hitbox_position.hpp"
#include "update_remove_after_lifetime.hpp"

inline void update(coord_t moveX, coord_t moveY, bool normalAttack, entt::registry &registry)
{
	inputUpdate(moveX, moveY, normalAttack, registry);
	positionUpdate(registry);
	resetCollisionFlags(registry);
	collisionUpdate(registry);
	updateHitboxPosition(registry);
	updateRemoveAfterLifetime(registry);
}

#endif // __UPDATE_HPP__
