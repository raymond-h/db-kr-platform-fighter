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
	int64_t lastStickExtremeFrame;

	void updateFrameCounters(int64_t frameCounter)
	{
		currentFrame = frameCounter;
		if (isMoveStickNeutral())
		{
			lastStickNeutralFrame = frameCounter;
		}
		else if (isMoveStickExtreme())
		{
			lastStickExtremeFrame = frameCounter;
		}
	}

	bool isMoveStickNeutral() const
	{
		return (moveX * moveX) + (moveY * moveY) <= (DEADZONE * DEADZONE);
	}

	bool isMoveStickExtreme() const
	{
		return (moveX * moveX) + (moveY * moveY) > (STRONG_POS_LIMIT * STRONG_POS_LIMIT);
	}

	bool isStrongDirectional() const
	{
		return (lastStickExtremeFrame - lastStickNeutralFrame) <= 4 &&
		       (currentFrame - lastStickExtremeFrame) <= 3;
	}
};

std::ostream &operator<<(std::ostream &os, const InputData &inputData);

struct CompleteInputData
{
	InputData inputDatas[4];
};

std::ostream &operator<<(std::ostream &os, const CompleteInputData &completeInputData);

#endif// __INPUT_HPP__
