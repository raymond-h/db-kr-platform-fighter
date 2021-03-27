#ifndef __PREFABS_HPP__
#define __PREFABS_HPP__

#include "components.hpp"

inline auto
createPlayer(entt::registry &registry, int16_t playerIndex, std::reference_wrapper<const Data::Character> character, coord_t x, coord_t y, coord_t w,
	coord_t h)
{
	auto playerEntity = registry.create();
	registry.emplace<PlayerControllable>(playerEntity, playerIndex);
	registry.emplace<FighterState>(playerEntity, FACING_RIGHT, FighterStateEnum::Idle, 0, 0, 0);
	registry.emplace<FighterData>(playerEntity, character);
	registry.emplace<FighterInput>(playerEntity, (int16_t)0, (int16_t)0, false, false, false);
	registry.emplace<Position>(playerEntity, x, y);
	registry.emplace<CollisionBox>(playerEntity, w, h);
	registry.emplace<Velocity>(playerEntity, 0, 0);
	registry.emplace<GroundCollisionFlags>(playerEntity, false, false, false, false);
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

#endif// __PREFABS_HPP__
