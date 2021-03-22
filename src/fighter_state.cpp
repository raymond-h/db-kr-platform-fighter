#include "fighter_state.hpp"

#define FSE_STR(s)            \
    case FighterStateEnum::s: \
        return #s;

std::ostream &operator<<(std::ostream &os, const FighterStateEnum &fse)
{
    os << fighter_state_enum_to_string(fse);
    return os;
}

const char *fighter_state_enum_to_string(const FighterStateEnum &fse)
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
        FSE_STR(ForwardSmash)
        FSE_STR(UpSmash)
        FSE_STR(DownSmash)
    }
    return nullptr;
}
