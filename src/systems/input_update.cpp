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
            return FighterStateEnum::UpSmashCharge;
        }
        else if (isDown && isStrong && fighterInput.doNormalAttack)
        {
            return FighterStateEnum::DownSmashCharge;
        }
        else if (isHoldingXDir && isStrong)
        {
            if (fighterInput.doNormalAttack)
            {
                return FighterStateEnum::ForwardSmashCharge;
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
        if (!fighterInput.doNormalAttack || fs.currentStateFrameCounter >= 150)
        {
            return FighterStateEnum::ForwardSmashRelease;
        }
    }
    else if (state == FighterStateEnum::ForwardSmashRelease && fs.currentStateFrameCounter >= 40)
    {
        return FighterStateEnum::Idle;
    }
    // upsmash
    else if (state == FighterStateEnum::UpSmashCharge)
    {
        if (!fighterInput.doNormalAttack || fs.currentStateFrameCounter >= 150)
        {
            return FighterStateEnum::UpSmashRelease;
        }
    }
    else if (state == FighterStateEnum::UpSmashRelease && fs.currentStateFrameCounter >= 40)
    {
        return FighterStateEnum::Idle;
    }
    // downsmash
    else if (state == FighterStateEnum::DownSmashCharge)
    {
        if (!fighterInput.doNormalAttack || fs.currentStateFrameCounter >= 150)
        {
            return FighterStateEnum::DownSmashRelease;
        }
    }
    else if (state == FighterStateEnum::DownSmashRelease && fs.currentStateFrameCounter >= 40)
    {
        return FighterStateEnum::Idle;
    }

    return state; // default to same state
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
        return FighterStateEnum::ForwardSmashCharge;
    }
    else
    {
        return computeNextState(fs, fighterInput, vel, gColFlags);
    }
}

void updateChara(const FighterState &fs, const FighterInput &fighterInput, Velocity &vel)
{
    const auto isHoldingXDir = fighterInput.isHoldingXDirection();
    const auto moveX = !isHoldingXDir ? 0 : Fixed(fighterInput.moveX) / 32768;

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
