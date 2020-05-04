#ifndef __COMPONENTS_HPP__
#define __COMPONENTS_HPP__

#include <iostream>
#include <entt/entt.hpp>

#include "fixed.hpp"
#include "aabb.hpp"

typedef Fixed coord_t;

enum class FighterStateEnum
{
	Idle,
	Walking,
	Dashing,
	Jumping,
	Falling
};

#define FSE_STR(s)            \
	case FighterStateEnum::s: \
		os << #s;             \
		break;

std::ostream &operator<<(std::ostream &os, const FighterStateEnum &fse)
{
	switch (fse)
	{
		FSE_STR(Idle)
		FSE_STR(Walking)
		FSE_STR(Dashing)
		FSE_STR(Jumping)
		FSE_STR(Falling)
	}
	return os;
}

bool isGroundMoving(FighterStateEnum fse)
{
	return fse == FighterStateEnum::Walking || fse == FighterStateEnum::Dashing;
}

bool isGrounded(FighterStateEnum fse)
{
	return fse == FighterStateEnum::Idle || isGroundMoving(fse);
}

bool isAirborne(FighterStateEnum fse)
{
	return fse == FighterStateEnum::Jumping || fse == FighterStateEnum::Falling;
}

bool isJumpable(FighterStateEnum fse)
{
	return isGrounded(fse);
}

struct PlayerControllable
{
};

struct FighterState
{
	FighterStateEnum fighterState;
	int64_t currentStateFrameCounter;
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

inline AABB boxForEntity(Position &pos, CollisionBox &cbox)
{
	return {
		pos.x - cbox.width / 2, pos.y - cbox.height / 2,
		pos.x + cbox.width / 2, pos.y + cbox.height / 2};
}

#endif // __COMPONENTS_HPP__
