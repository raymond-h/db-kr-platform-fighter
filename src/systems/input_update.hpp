#ifndef __INPUT_UPDATE_HPP__
#define __INPUT_UPDATE_HPP__

#include <entt/entt.hpp>

#include "components.hpp"

inline void inputUpdate(coord_t moveX, coord_t moveY, entt::registry &registry)
{
	auto players = registry.view<PlayerControllable, Velocity, GroundCollisionFlags>();
	for (auto &entity : players)
	{
		auto [vel, gColFlags] = registry.get<Velocity, GroundCollisionFlags>(entity);

		vel.x = moveX * 5 / 2;
		if (gColFlags.bottom && moveY < 0)
		{
			vel.y = -5;
		}
	}
}

#endif // __INPUT_UPDATE_HPP__
