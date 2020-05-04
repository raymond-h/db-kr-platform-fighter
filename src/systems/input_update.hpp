#ifndef __INPUT_UPDATE_HPP__
#define __INPUT_UPDATE_HPP__

#include <variant>
#include <entt/entt.hpp>

#include "components.hpp"

// const int16_t STRONG_LIMIT = 15000;
const int16_t STRONG_LIMIT = 5000;
const int16_t DEADZONE = 3300;

struct InputData
{
	int16_t moveX, moveY;
	int16_t moveVelX, moveVelY;
	bool doJump;

	bool isStrongDirectional() const
	{
		return (moveVelX * moveVelX) + (moveVelY * moveVelY) > (STRONG_LIMIT * STRONG_LIMIT);
	}
};

inline auto computeNextState(
	const FighterStateEnum &state,
	const InputData &inputData,
	const Velocity &vel,
	const GroundCollisionFlags &gColFlags)
{
	const auto isHoldingDir = abs(inputData.moveX) >= DEADZONE;

	if (isAirborne(state))
	{
		if (gColFlags.bottom)
		{
			return isHoldingDir ? FighterStateEnum::Walking : FighterStateEnum::Idle;
		}
		else if (vel.y > 0)
		{
			return FighterStateEnum::Falling;
		}
	}
	else if (state == FighterStateEnum::Idle)
	{
		if (inputData.doJump && gColFlags.bottom)
		{
			return FighterStateEnum::Jumping;
		}
		else if (isHoldingDir && inputData.isStrongDirectional())
		{
			return FighterStateEnum::Dashing;
		}
		else if (isHoldingDir)
		{
			return FighterStateEnum::Walking;
		}
	}
	else if (isGroundMoving(state))
	{
		if (inputData.doJump && gColFlags.bottom)
		{
			return FighterStateEnum::Jumping;
		}
		else if (!isHoldingDir)
		{
			return FighterStateEnum::Idle;
		}
	}

	return state; // default to same state
}

inline void inputUpdate(const InputData &inputData, entt::registry &registry)
{
	const auto isHoldingDir = abs(inputData.moveX) >= DEADZONE;
	const auto moveX = !isHoldingDir ? 0 : Fixed(inputData.moveX) / 32768;

	auto players = registry.view<PlayerControllable, FighterState, Velocity, GroundCollisionFlags>();
	for (auto &entity : players)
	{
		auto [fs, vel, gColFlags] = registry.get<FighterState, Velocity, GroundCollisionFlags>(entity);

		auto nextState = computeNextState(fs.fighterState, inputData, vel, gColFlags);

		if (nextState != fs.fighterState)
		{
			fs.fighterState = nextState;
			fs.currentStateFrameCounter = 0;
		}
		else
		{
			fs.currentStateFrameCounter++;
		}

		// std::cout << fs.fighterState << std::endl;

		// update vel from state
		if (fs.fighterState == FighterStateEnum::Idle)
		{
			vel.x = 0;
		}
		else if (isGroundMoving(fs.fighterState))
		{
			vel.x = fs.fighterState == FighterStateEnum::Dashing ? (moveX * 5) / 2 : (moveX * 5) / 4;
		}
		else if (isAirborne(fs.fighterState))
		{
			vel.x = (moveX * 5) / 4;

			if (fs.fighterState == FighterStateEnum::Jumping && fs.currentStateFrameCounter == 0)
			{
				vel.y = -5;
			}
		}
	}
}

#endif // __INPUT_UPDATE_HPP__
