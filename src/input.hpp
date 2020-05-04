#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include <iostream>

#include "components.hpp"

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

    bool isStrongDirectional() const
    {
        const auto isPastStrongLimit = (moveX * moveX) + (moveY * moveY) > (STRONG_POS_LIMIT * STRONG_POS_LIMIT);
        return isPastStrongLimit && (currentFrame - lastStickNeutralFrame) <= 4;
    }
};

std::ostream &operator<<(std::ostream &os, const InputData &inputData);

struct CompleteInputData
{
    InputData inputDatas[4];
};

std::ostream &operator<<(std::ostream &os, const CompleteInputData &completeInputData);

#endif // __INPUT_HPP__
