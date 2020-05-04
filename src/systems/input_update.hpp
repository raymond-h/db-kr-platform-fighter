#ifndef __INPUT_UPDATE_HPP__
#define __INPUT_UPDATE_HPP__

#include <variant>
#include <entt/entt.hpp>

#include "components.hpp"

const int16_t DEADZONE = 3300;
const int16_t STRONG_POS_LIMIT = 25000;

struct InputData
{
	int16_t moveX, moveY;
	bool doJump;
	bool doNormalAttack;

	int64_t currentFrame;
	int64_t lastStickNeutralFrame;

	void updateFrameCounters(int64_t frameCounter)
	{
		currentFrame = frameCounter;
		if (isMoveStickNeutral())
		{
			lastStickNeutralFrame = frameCounter;
		}
	}

	bool isMoveStickNeutral() const
	{
		return (moveX * moveX) + (moveY * moveY) <= (DEADZONE * DEADZONE);
	}

	bool isHoldingDirection() const
	{
		return abs(moveX) >= DEADZONE;
	}

	bool isStrongDirectional() const
	{
		const auto isPastStrongLimit = (moveX * moveX) + (moveY * moveY) > (STRONG_POS_LIMIT * STRONG_POS_LIMIT);
		return isPastStrongLimit && (currentFrame - lastStickNeutralFrame) <= 4;
	}
};

std::ostream &operator<<(std::ostream &os, const InputData &inputData)
{
	os << "{ moveX: " << inputData.moveX
	   << "; moveY: " << inputData.moveY
	   << "; doJump: " << inputData.doJump
	   << "; doNormalAttack: " << inputData.doNormalAttack
	   << "; }";
	return os;
}

struct CompleteInputData
{
	InputData inputDatas[4];
};

struct GameInputData
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

GameInputData toGameInputData(const InputData &inputData)
{
	return {
		inputData.moveX,
		inputData.moveY,
		inputData.isStrongDirectional(),
		inputData.doJump,
		inputData.doNormalAttack};
}

inline auto computeNextState(
	const FighterState &fs,
	const GameInputData &inputData,
	const Velocity &vel,
	const GroundCollisionFlags &gColFlags)
{
	const FighterStateEnum &state = fs.fighterState;
	const auto isHoldingDir = inputData.isHoldingDirection();
	const auto isStrong = inputData.isStrong;

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
		else if (isHoldingDir && isStrong)
		{
			if (inputData.doNormalAttack)
			{
				return FighterStateEnum::ForwardSmashCharge;
			}
			else
			{
				return FighterStateEnum::Dashing;
			}
		}
		else if (isHoldingDir)
		{
			if (inputData.doNormalAttack)
			{
				return FighterStateEnum::ForwardTilt;
			}
			else
			{
				return FighterStateEnum::Walking;
			}
		}
		else if (inputData.doNormalAttack)
		{
			return FighterStateEnum::Jab;
		}
	}
	else if (isGroundMoving(state))
	{
		if (inputData.doJump && gColFlags.bottom)
		{
			return FighterStateEnum::Jumping;
		}
		else if (state == FighterStateEnum::Walking && inputData.doNormalAttack)
		{
			return FighterStateEnum::ForwardTilt;
		}
		else if (state == FighterStateEnum::Dashing && inputData.doNormalAttack)
		{
			return FighterStateEnum::DashAttack;
		}
		else if (!isHoldingDir)
		{
			return FighterStateEnum::Idle;
		}
	}

	// attacks
	else if (state == FighterStateEnum::Jab && fs.currentStateFrameCounter >= 10)
	{
		return FighterStateEnum::Idle;
	}
	else if (state == FighterStateEnum::ForwardTilt && fs.currentStateFrameCounter >= 25)
	{
		return FighterStateEnum::Idle;
	}
	else if (state == FighterStateEnum::DashAttack && fs.currentStateFrameCounter >= 50)
	{
		return FighterStateEnum::Idle;
	}
	else if (state == FighterStateEnum::ForwardSmashCharge)
	{
		if (!inputData.doNormalAttack || fs.currentStateFrameCounter >= 150)
		{
			return FighterStateEnum::ForwardSmashRelease;
		}
	}
	else if (state == FighterStateEnum::ForwardSmashRelease && fs.currentStateFrameCounter >= 40)
	{
		return FighterStateEnum::Idle;
	}

	return state; // default to same state
}

inline auto computeNextStateEarlyCancel(
	const FighterState &fs,
	const GameInputData &inputData,
	const Velocity &vel,
	const GroundCollisionFlags &gColFlags)
{
	const FighterStateEnum &state = fs.fighterState;
	const auto isStrong = inputData.isStrong;

	if (state == FighterStateEnum::Walking && fs.currentStateFrameCounter <= 2 && isStrong)
	{
		return FighterStateEnum::Dashing;
	}
	else if (state == FighterStateEnum::Dashing && fs.currentStateFrameCounter <= 2 && inputData.doNormalAttack)
	{
		return FighterStateEnum::ForwardSmashCharge;
	}
	else
	{
		return computeNextState(fs, inputData, vel, gColFlags);
	}
}

inline void updateChara(const FighterState &fs, const GameInputData &gameInputData, Velocity &vel)
{
	const auto isHoldingDir = gameInputData.isHoldingDirection();
	const auto moveX = !isHoldingDir ? 0 : Fixed(gameInputData.moveX) / 32768;

	if (fs.fighterState == FighterStateEnum::Idle || isChargingSmashAttack(fs.fighterState))
	{
		vel.x = 0;
	}
	else if (isGroundMoving(fs.fighterState))
	{
		vel.x = fs.fighterState == FighterStateEnum::Dashing ? (moveX * 5) / 2 : (moveX * 5) / 4;
	}
	else if (fs.fighterState == FighterStateEnum::DashAttack)
	{
		const auto dir = moveX > 0 ? 1 : -1;
		vel.x = Fixed(8) / 2 * dir;
	}
	else if (isAirborne(fs.fighterState))
	{
		vel.x = (moveX * 5) / 4;

		if (fs.fighterState == FighterStateEnum::Jumping && fs.currentStateFrameCounter == 0)
		{
			vel.y = -5;
		}
	}
};

inline void inputUpdate(const CompleteInputData &completeInputData, entt::registry &registry)
{
	const auto frameCounter = registry.ctx<int64_t>();

	auto players = registry.view<PlayerControllable, FighterState, Velocity, GroundCollisionFlags>();
	for (auto &entity : players)
	{
		auto [pCon, fs, vel, gColFlags] = registry.get<PlayerControllable, FighterState, Velocity, GroundCollisionFlags>(entity);

		const auto inputData = completeInputData.inputDatas[pCon.playerIndex];

		const auto gameInputData = toGameInputData(inputData);

		auto nextState = computeNextStateEarlyCancel(fs, gameInputData, vel, gColFlags);

		if (nextState != fs.fighterState)
		{
			fs.fighterState = nextState;
			fs.currentStateFrameCounter = 0;
		}
		else
		{
			fs.currentStateFrameCounter++;
		}

		std::cout << "[" << frameCounter << "] " << fs.fighterState << " - Inputs: " << inputData << std::endl;

		// update vel from state
		updateChara(fs, gameInputData, vel);
	}
}

#endif // __INPUT_UPDATE_HPP__
