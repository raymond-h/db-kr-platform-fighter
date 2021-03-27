#include <optional>

#include "input_update.hpp"

void assignFighterInputs(const InputData &inputData, FighterInput &fi)
{
	fi.moveX = inputData.moveX;
	fi.moveY = inputData.moveY;
	fi.doJump = inputData.doJump;
	fi.doNormalAttack = inputData.doNormalAttack;
	fi.isStrong = inputData.isStrongDirectional();
}

FighterStateEnum computeNextState(
	const FighterState &fs,
	const FighterInput &fighterInput,
	const Velocity &vel,
	const GroundCollisionFlags &gColFlags)
{
	return computeNextStateEarlyCancel(fs, fighterInput, vel, gColFlags);
}

FighterStateEnum computeNextStateStandard(
	const FighterState &fs,
	const FighterInput &fighterInput,
	const Velocity &vel,
	const GroundCollisionFlags &gColFlags)
{
	const FighterStateEnum &state = fs.fighterState;
	const auto isHoldingXDir = fighterInput.isHoldingXDirection();
	const auto isHoldingYDir = fighterInput.isHoldingYDirection();
	const auto isStrong = fighterInput.isStrong;

	const auto isUp = isHoldingYDir && fighterInput.moveY < 0;
	const auto isDown = isHoldingYDir && fighterInput.moveY > 0;

	if (isAirborne(state))
	{
		if (gColFlags.bottom)
		{
			return isHoldingXDir ? FighterStateEnum::Walking : FighterStateEnum::Idle;
		}
		else if (vel.y > 0)
		{
			return FighterStateEnum::Falling;
		}
	}
	else if (state == FighterStateEnum::Idle)
	{
		if (fighterInput.doJump && gColFlags.bottom)
		{
			return FighterStateEnum::Jumping;
		}
		else if (isUp && isStrong && fighterInput.doNormalAttack)
		{
			return FighterStateEnum::UpSmash;
		}
		else if (isDown && isStrong && fighterInput.doNormalAttack)
		{
			return FighterStateEnum::DownSmash;
		}
		else if (isHoldingXDir && isStrong)
		{
			if (fighterInput.doNormalAttack)
			{
				return FighterStateEnum::ForwardSmash;
			}
			else
			{
				return FighterStateEnum::Dashing;
			}
		}
		else if (isHoldingXDir)
		{
			if (fighterInput.doNormalAttack)
			{
				return FighterStateEnum::ForwardTilt;
			}
			else
			{
				return FighterStateEnum::Walking;
			}
		}
		else if (fighterInput.doNormalAttack)
		{
			return FighterStateEnum::Jab;
		}
	}
	else if (isGroundMoving(state))
	{
		if (fighterInput.doJump && gColFlags.bottom)
		{
			return FighterStateEnum::Jumping;
		}
		else if (state == FighterStateEnum::Walking && fighterInput.doNormalAttack)
		{
			return FighterStateEnum::ForwardTilt;
		}
		else if (state == FighterStateEnum::Dashing && fighterInput.doNormalAttack)
		{
			return FighterStateEnum::DashAttack;
		}
		else if (!isHoldingXDir)
		{
			return FighterStateEnum::Idle;
		}
	}

	return state;
}

FighterStateEnum computeNextStateAttackMove(
	const FighterState &fs,
	const FighterInput &fighterInput,
	const Velocity &vel,
	const GroundCollisionFlags &gColFlags)
{
	return computeNextStateStandard(fs, fighterInput, vel, gColFlags);
}

FighterStateEnum computeNextStateEarlyCancel(
	const FighterState &fs,
	const FighterInput &fighterInput,
	const Velocity &vel,
	const GroundCollisionFlags &gColFlags)
{
	const FighterStateEnum &state = fs.fighterState;
	const auto isStrong = fighterInput.isStrong;

	if (state == FighterStateEnum::Walking && fs.currentStateFrameCounter <= 2 && isStrong)
	{
		return FighterStateEnum::Dashing;
	}
	else if (state == FighterStateEnum::Dashing && fs.currentStateFrameCounter <= 2 && fighterInput.doNormalAttack)
	{
		return FighterStateEnum::ForwardSmash;
	}
	else
	{
		return computeNextStateAttackMove(fs, fighterInput, vel, gColFlags);
	}
}

std::optional<Data::MoveType> fighterStateToMoveType(FighterStateEnum state)
{
	switch (state)
	{
	case FighterStateEnum::Jab:
		return Data::MoveType::Jab;
	case FighterStateEnum::DashAttack:
		return Data::MoveType::DashAttack;
	case FighterStateEnum::ForwardTilt:
		return Data::MoveType::ForwardTilt;
	case FighterStateEnum::ForwardSmash:
		return Data::MoveType::ForwardSmash;
	case FighterStateEnum::UpSmash:
		return Data::MoveType::UpSmash;
	case FighterStateEnum::DownSmash:
		return Data::MoveType::DownSmash;

	default:
		return {};
	}
}

std::variant<WindowChange, FighterStateEnum>
computeNextWindow(const FighterState &fs, const FighterInput &fighterInput, const FighterData &data)
{
	if (!isAttacking(fs.fighterState))
	{
		return WindowChange{fs.window};
	}

	const auto &state = fs.fighterState;
	const auto &window = fs.window;

	if (auto moveType = fighterStateToMoveType(state))
	{
		const Data::Character &characterData = data.character;
		const Data::Move &moveData = characterData.moves.at(*moveType);
		const Data::Window &currentWindowData = moveData.windows[window];

		if (fs.currentWindowFrameCounter >= currentWindowData.getLength())
		{
			if (window + 1 >= moveData.windows.size())
			{
				// TODO Specify this as part of move data? To allow special fall from certain moves
				return FighterStateEnum::Idle;
			}
			else if (currentWindowData.getLoopType() == Data::WindowLoopType::LoopUntilButtonLetGo &&
				fighterInput.doNormalAttack && fs.currentWindowFrameCounter < 150)
			{
				return WindowChange{window};
			}
			else
			{
				return WindowChange{window + 1};
			}
		}
	}

	return WindowChange{window};
}

void updateChara(const FighterState &fs, const FighterInput &fighterInput, Velocity &vel, facing_t &facing,
	const FighterData &data)
{
	const auto isHoldingXDir = fighterInput.isHoldingXDirection();
	const auto moveX = !isHoldingXDir ? 0 : Fixed(fighterInput.moveX) / 32768;

	if (fs.fighterState == FighterStateEnum::Idle || isAttackingStill(fs.fighterState))
	{
		vel.x = 0;
	}
	else if (isGroundMoving(fs.fighterState))
	{
		facing = moveX.sign().int_value();
		vel.x = fs.fighterState == FighterStateEnum::Dashing ? (moveX * 5) / 2 : (moveX * 5) / 4;
	}
	else if (auto moveType = fighterStateToMoveType(fs.fighterState))
	{
		const auto &window = fs.window;
		const auto &characterData = data.character;
		const auto &moveData = characterData.get().moves.at(*moveType);
		const auto &currentWindowData = moveData.windows[window];

		vel.x = currentWindowData.getMoveX() * fs.facing;
		vel.y = currentWindowData.getMoveY();
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
