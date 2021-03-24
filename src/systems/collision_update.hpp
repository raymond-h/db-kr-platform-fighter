#ifndef __COLLISION_UPDATE_HPP__
#define __COLLISION_UPDATE_HPP__

#include <entt/entt.hpp>

#include "components.hpp"

inline void
handleGroundCollision(Position &aPos, const CollisionBox &aCbox, Position &bPos, const CollisionBox &bCbox,
	GroundCollisionFlags *bGColFlags)
{
	auto aAabb = boxForEntity(aPos, aCbox);
	auto bAabb = boxForEntity(bPos, bCbox);

	coord_t overlapX = 0, overlapY = 0;
	calculateOverlap(aAabb, bAabb, overlapX, overlapY);

	if (abs(overlapX) < abs(overlapY))
	{
		bPos.x += overlapX;
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
		bPos.y += overlapY;
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

inline void collisionUpdate(entt::registry &registry)
{
	auto ents = registry.view<Position, const CollisionBox>();
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
			auto[aPos, aCbox] = ents.get<Position, const CollisionBox>(a);
			auto[bPos, bCbox] = ents.get<Position, const CollisionBox>(b);

			if (aIsGround && !bIsGround)
			{
				auto bColFlags = registry.try_get<GroundCollisionFlags>(b);
				handleGroundCollision(aPos, aCbox, bPos, bCbox, bColFlags);
			}
			else if (!aIsGround && bIsGround)
			{
				auto aColFlags = registry.try_get<GroundCollisionFlags>(a);
				handleGroundCollision(bPos, bCbox, aPos, aCbox, aColFlags);
			}
		}
	}
}

#endif// __COLLISION_UPDATE_HPP__
