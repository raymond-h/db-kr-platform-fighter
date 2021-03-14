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

inline bool constexpr isAttackingStill(FighterStateEnum fse)
{
    return fse == FighterStateEnum::Jab ||
           fse == FighterStateEnum::ForwardTilt ||
           fse == FighterStateEnum::ForwardSmashCharge ||
           fse == FighterStateEnum::ForwardSmashRelease ||
           fse == FighterStateEnum::UpSmashCharge ||
           fse == FighterStateEnum::UpSmashRelease ||
           fse == FighterStateEnum::DownSmashCharge ||
           fse == FighterStateEnum::DownSmashRelease;
}

inline bool constexpr isAttacking(FighterStateEnum fse)
{
    return isAttackingStill(fse) ||
           fse == FighterStateEnum::DashAttack;
}

#endif // __FIGHTER_STATE_HPP__
