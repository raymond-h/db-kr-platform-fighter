#ifndef __COMPONENTS_HPP__
#define __COMPONENTS_HPP__

struct PlayerControllable
{
};

struct Position
{
	int32_t x;
	int32_t y;
};

struct CollisionBox
{
	int32_t width;
	int32_t height;
};

struct GroundCollisionFlags
{
	bool left, top, right, bottom;
};

struct Velocity
{
	int32_t x;
	int32_t y;
};

struct Ground
{
};

#endif // __COMPONENTS_HPP__
