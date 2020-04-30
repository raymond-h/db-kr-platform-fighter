#ifndef __PREFABS_HPP__
#define __PREFABS_HPP__

#include "components.hpp"

inline auto createPlayer(entt::registry &registry, coord_t x, coord_t y, coord_t w, coord_t h)
{
	auto playerEntity = registry.create();
	registry.emplace<PlayerControllable>(playerEntity);
	registry.emplace<Position>(playerEntity, x, y);
	registry.emplace<CollisionBox>(playerEntity, w, h);
	registry.emplace<Velocity>(playerEntity, 0, 0);
	registry.emplace<GroundCollisionFlags>(playerEntity, false, false, false, false);
	registry.emplace<Facing>(playerEntity, true);
	return playerEntity;
}

inline auto createNPC(entt::registry &registry, coord_t x, coord_t y, coord_t w, coord_t h)
{
	auto playerEntity = registry.create();
	registry.emplace<Position>(playerEntity, x, y);
	registry.emplace<CollisionBox>(playerEntity, w, h);
	registry.emplace<Velocity>(playerEntity, 0, 0);
	registry.emplace<GroundCollisionFlags>(playerEntity, false, false, false, false);
	registry.emplace<Facing>(playerEntity, true);
	// registry.emplace<RemoveAfterLifetime>(playerEntity, 600);
	return playerEntity;
}

inline auto createGround(entt::registry &registry, coord_t x, coord_t y, coord_t w, coord_t h)
{
	auto ent = registry.create();
	registry.emplace<Position>(ent, x, y);
	registry.emplace<CollisionBox>(ent, w, h);
	registry.emplace<Ground>(ent);
	return ent;
}

inline entt::entity createHitbox(
	entt::registry &registry,
	entt::entity ownerEntity,
	coord_t offsetX, coord_t offsetY,
	coord_t w, coord_t h,
	coord_t impulseX, coord_t impulseY,
	int32_t lifetime)
{
	auto pos = registry.get<Position>(ownerEntity);

	auto ent = registry.create();
	registry.emplace<Position>(ent, pos.x + offsetX, pos.y + offsetY);
	registry.emplace<CollisionBox>(ent, w, h);
	registry.emplace<Hitbox>(ent, ownerEntity, offsetX, offsetY, impulseX, impulseY);
	registry.emplace<RemoveAfterLifetime>(ent, lifetime);
	return ent;
}

#endif // __PREFABS_HPP__
