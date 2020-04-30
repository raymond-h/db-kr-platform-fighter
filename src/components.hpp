#ifndef __COMPONENTS_HPP__
#define __COMPONENTS_HPP__

#include <entt/entt.hpp>

#include "fixed.hpp"
#include "aabb.hpp"

#define SNAPSHOT_COMPONENTS                     \
    PlayerControllable, Position, CollisionBox, \
        GroundCollisionFlags, Velocity, NetSynced

#define SERIALIZABLE(...)       \
	template <class Archive>    \
	void serialize(Archive &ar) \
	{                           \
		ar(__VA_ARGS__);        \
	}

typedef Fixed coord_t;

struct PlayerControllable
{
};

struct Position
{
	coord_t x;
	coord_t y;

	SERIALIZABLE(x, y)
};

struct CollisionBox
{
	coord_t width;
	coord_t height;

	SERIALIZABLE(width, height)
};

struct GroundCollisionFlags
{
	bool left, top, right, bottom;

	SERIALIZABLE(left, top, right, bottom)
};

struct Velocity
{
	coord_t x;
	coord_t y;

	SERIALIZABLE(x, y)
};

struct Ground
{
};

struct NetSynced
{
};

inline AABB boxForEntity(Position& pos, CollisionBox& cbox)
{
	return {
		pos.x - cbox.width / 2, pos.y - cbox.height / 2,
		pos.x + cbox.width / 2, pos.y + cbox.height / 2 };
}

#endif // __COMPONENTS_HPP__
