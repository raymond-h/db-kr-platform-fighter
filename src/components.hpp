#ifndef __COMPONENTS_HPP__
#define __COMPONENTS_HPP__

#include <entt/entt.hpp>

#include "fixed.hpp"
#include "aabb.hpp"
#include "fighter_state.hpp"

typedef Fixed coord_t;

struct PlayerControllable
{
	int16_t playerIndex;
};

struct FighterState
{
	FighterStateEnum fighterState;
	int64_t currentStateFrameCounter;
};

const int16_t DEADZONE = 3300;
const int16_t STRONG_POS_LIMIT = 25000;

struct FighterInput
{
	int16_t moveX, moveY;
	bool isStrong;
	bool doJump;
	bool doNormalAttack;

	bool isMoveStickNeutral() const
	{
		return (moveX * moveX) + (moveY * moveY) <= (DEADZONE * DEADZONE);
	}

	bool isHoldingDirection() const
	{
		return abs(moveX) >= DEADZONE;
	}
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
