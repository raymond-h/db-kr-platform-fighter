#include "fighter_state.hpp"

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
        FSE_STR(Jab)
        FSE_STR(DashAttack)
        FSE_STR(ForwardTilt)
        FSE_STR(ForwardSmashCharge)
        FSE_STR(ForwardSmashRelease)
        FSE_STR(UpSmashCharge)
        FSE_STR(UpSmashRelease)
        FSE_STR(DownSmashCharge)
        FSE_STR(DownSmashRelease)
    }
    return os;
}
