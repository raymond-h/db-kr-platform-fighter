#include "input.hpp"

std::ostream &operator<<(std::ostream &os, const InputData &inputData)
{
    os << "{ moveX: " << inputData.moveX
       << "; moveY: " << inputData.moveY
       << "; doJump: " << inputData.doJump
       << "; doNormalAttack: " << inputData.doNormalAttack
       << "; isStrongDirectional(): " << inputData.isStrongDirectional()
       << "; }";
    return os;
}

std::ostream &operator<<(std::ostream &os, const CompleteInputData &completeInputData)
{
    os << "{ Player 1: " << completeInputData.inputDatas[0]
       << "; Player 2: " << completeInputData.inputDatas[1]
       << "; Player 3: " << completeInputData.inputDatas[2]
       << "; Player 4: " << completeInputData.inputDatas[3]
       << "; }";
    return os;
}
