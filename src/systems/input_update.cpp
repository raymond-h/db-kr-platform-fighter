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

std::variant<WindowChange, FighterStateEnum> computeNextWindow(
    const FighterState &fs,
    const FighterInput &fighterInput)
{
    if (!isAttacking(fs.fighterState))
    {
        return WindowChange{fs.window};
    }

    const auto &state = fs.fighterState;
    const auto &window = fs.window;

    if (state == FighterStateEnum::Jab)
    {
        if (window == 0 && fs.currentWindowFrameCounter >= 3)
        {
            return WindowChange{1};
        }
        else if (window == 1 && fs.currentWindowFrameCounter >= 4)
        {
            return WindowChange{2};
        }
        else if (window == 2 && fs.currentWindowFrameCounter >= 3)
        {
            return FighterStateEnum::Idle;
        }
    }
    else if (state == FighterStateEnum::ForwardTilt)
    {
        if (window == 0 && fs.currentWindowFrameCounter >= 10)
        {
            return WindowChange{1};
        }
        else if (window == 1 && fs.currentWindowFrameCounter >= 5)
        {
            return WindowChange{2};
        }
        else if (window == 2 && fs.currentWindowFrameCounter >= 10)
        {
            return FighterStateEnum::Idle;
        }
    }
    else if (state == FighterStateEnum::DashAttack && fs.currentStateFrameCounter >= 50)
    {
        if (window == 0 && fs.currentWindowFrameCounter >= 15)
        {
            return WindowChange{1};
        }
        else if (window == 1 && fs.currentWindowFrameCounter >= 35)
        {
            return FighterStateEnum::Idle;
        }
    }
    // forwardsmash
    else if (state == FighterStateEnum::ForwardSmash)
    {
        if (window == 0 && (!fighterInput.doNormalAttack || fs.currentWindowFrameCounter >= 150))
        {
            return WindowChange{1};
        }
        else if (window == 1 && fs.currentWindowFrameCounter >= 40)
        {
            return FighterStateEnum::Idle;
        }
    }
    // upsmash
    else if (state == FighterStateEnum::UpSmash)
    {
        if (window == 0 && (!fighterInput.doNormalAttack || fs.currentWindowFrameCounter >= 150))
        {
            return WindowChange{1};
        }
        else if (window == 1 && fs.currentWindowFrameCounter >= 40)
        {
            return FighterStateEnum::Idle;
        }
    }
    // downsmash
    else if (state == FighterStateEnum::DownSmash)
    {
        if (window == 0 && (!fighterInput.doNormalAttack || fs.currentWindowFrameCounter >= 150))
        {
            return WindowChange{1};
        }
        else if (window == 1 && fs.currentWindowFrameCounter >= 40)
        {
            return FighterStateEnum::Idle;
        }
    }

    return WindowChange{window};
}

void updateChara(const FighterState &fs, const FighterInput &fighterInput, Velocity &vel, facing_t &facing)
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
    else if (fs.fighterState == FighterStateEnum::DashAttack)
    {
        vel.x = Fixed(8) / 2 * fs.facing;
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
