#ifndef __COMPONENTS_HPP__
#define __COMPONENTS_HPP__

#include <entt/entt.hpp>

#include "fixed.hpp"
#include "aabb.hpp"

typedef Fixed coord_t;

struct PlayerControllable
{
};

struct Position
{
	coord_t x;
	coord_t y;
};

struct CollisionBox
{
	coord_t width;
	coord_t height;
};

struct GroundCollisionFlags
{
	bool left, top, right, bottom;
};

struct Velocity
{
	coord_t x;
	coord_t y;
};

struct Facing
{
	bool facingRight;
};

struct Ground
{
};

struct Hitbox
{
	entt::entity owner;
	coord_t ownerOffsetX, ownerOffsetY;
	coord_t impulseX, impulseY;
};

struct RemoveAfterLifetime
{
	int32_t lifetime;
};

inline AABB boxForEntity(Position& pos, CollisionBox& cbox)
{
	return {
		pos.x - cbox.width / 2, pos.y - cbox.height / 2,
		pos.x + cbox.width / 2, pos.y + cbox.height / 2 };
}

#endif // __COMPONENTS_HPP__
