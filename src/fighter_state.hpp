#ifndef __FIGHTER_STATE_HPP__
#define __FIGHTER_STATE_HPP__

#include <iostream>

enum class FighterStateEnum
{
    Idle,
    Walking,
    Dashing,
    Jumping,
    Falling,
    Jab,
    DashAttack,
    ForwardTilt,
    ForwardSmashCharge,
    ForwardSmashRelease,
    UpSmashCharge,
    UpSmashRelease,
    DownSmashCharge,
    DownSmashRelease
};

std::ostream &operator<<(std::ostream &os, const FighterStateEnum &fse);

inline bool constexpr isGroundMoving(FighterStateEnum fse)
{
    return fse == FighterStateEnum::Walking || fse == FighterStateEnum::Dashing;
}

inline bool constexpr isGrounded(FighterStateEnum fse)
{
    return fse == FighterStateEnum::Idle || isGroundMoving(fse);
}

inline bool constexpr isAirborne(FighterStateEnum fse)
{
    return fse == FighterStateEnum::Jumping || fse == FighterStateEnum::Falling;
}

inline bool constexpr isJumpable(FighterStateEnum fse)
{
    return isGrounded(fse);
}

inline bool constexpr isChargingSmashAttack(FighterStateEnum fse)
{
    return fse == FighterStateEnum::ForwardSmashCharge ||
           fse == FighterStateEnum::UpSmashCharge ||
           fse == FighterStateEnum::DownSmashCharge;
}

#endif // __FIGHTER_STATE_HPP__
