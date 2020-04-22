#ifndef __COMPONENTS_HPP__
#define __COMPONENTS_HPP__

#include "fixed.hpp"

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

struct Ground
{
};

#endif // __COMPONENTS_HPP__
