#ifndef __INPUT_UPDATE_HPP__
#define __INPUT_UPDATE_HPP__

#include <entt/entt.hpp>

#include "components.hpp"

inline void inputUpdate(coord_t moveX, coord_t moveY, bool normalAttack, entt::registry &registry)
{
	auto movablePlayers = registry.view<PlayerControllable, Velocity, GroundCollisionFlags>();
	for (auto &entity : movablePlayers)
	{
		auto [vel, gColFlags] = registry.get<Velocity, GroundCollisionFlags>(entity);
		auto facing = registry.try_get<Facing>(entity);

		if (facing != nullptr && moveX != 0)
		{
			facing->facingRight = moveX > 0;
		}

		vel.x = moveX * 5 / 2;
		if (gColFlags.bottom && moveY < 0)
		{
			vel.y = -5;
		}
	}

	auto hittingPlayers = registry.view<PlayerControllable, Position, Facing>();
	for (auto &entity : hittingPlayers)
	{
		auto facing = registry.get<Facing>(entity);

		if (normalAttack)
		{
			auto facingFactor = facing.facingRight ? 1 : -1;
			createHitbox(registry, entity, facingFactor * 14, 0, 8, 8, facingFactor * 3, -3, 3);
		}
	}
}

#endif // __INPUT_UPDATE_HPP__
