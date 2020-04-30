#ifndef __UPDATE_HPP__
#define __UPDATE_HPP__

#include <entt/entt.hpp>

#include "input_update.hpp"
#include "position_update.hpp"
#include "reset_collision_flags.hpp"
#include "collision_update.hpp"

inline void update(coord_t moveX, coord_t moveY, entt::registry &registry)
{
	inputUpdate(moveX, moveY, registry);
	positionUpdate(registry);
	resetCollisionFlags(registry);
	collisionUpdate(registry);
}

#endif // __UPDATE_HPP__
