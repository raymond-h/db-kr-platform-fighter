#ifndef __POSITION_UPDATE_HPP__
#define __POSITION_UPDATE_HPP__

#include <entt/entt.hpp>

#include "components.hpp"

inline void positionUpdate(entt::registry &registry)
{
	registry.view<Position, Velocity, GroundCollisionFlags>().each([](Position &pos, Velocity &vel, GroundCollisionFlags &gColFlags) {
		if (gColFlags.bottom && vel.y > 0)
		{
			vel.y = 0;
		}

		vel.y += Fixed(1) / 5;

		pos.x += vel.x;
		pos.y += vel.y;
	});
}

#endif // __POSITION_UPDATE_HPP__
