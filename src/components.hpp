#ifndef __COMPONENTS_HPP__
#define __COMPONENTS_HPP__

#include <entt/entt.hpp>

#include "fixed.hpp"
#include "aabb.hpp"
#include "fighter_state.hpp"
#include "approx_math.hpp"

typedef Fixed coord_t;
typedef int32_t facing_t;

const facing_t FACING_RIGHT = 1;
const facing_t FACING_LEFT = -1;

struct PlayerControllable
{
	int16_t playerIndex;
};

struct FighterState
{
	facing_t facing;

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

	bool isHoldingXDirection() const
	{
		const auto angle = moveAngle<4>();
		return !isMoveStickNeutral() && (angle == 0 || angle == 2);
	}

	bool isHoldingYDirection() const
	{
		const auto angle = moveAngle<4>();
		return !isMoveStickNeutral() && (angle == 1 || angle == 3);
	}

	template <int32_t ANGLE_COUNT>
	int32_t moveAngle() const
	{
		const int32_t angle = fxpt_atan2(moveY, moveX) * (ANGLE_COUNT * 2) / 65535;
		return ((angle + 1) / 2) % ANGLE_COUNT;
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
