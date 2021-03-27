#ifndef DBKRPLATFORMFIGHTER_INTERFACE_H
#define DBKRPLATFORMFIGHTER_INTERFACE_H

#include <vector>
#include <map>

#include "../fixed.hpp"

namespace Data
{
	struct Hitbox
	{
		const uint32_t startingWindowIndex;
		const uint32_t frameLifespan;

		Hitbox(uint32_t startingWindowIndex, uint32_t frameLifespan)
			: startingWindowIndex(startingWindowIndex), frameLifespan(frameLifespan) {}
	};

	enum class WindowLoopType
	{
		DoNotLoop,
		LoopUntilButtonLetGo
	};

	struct Window
	{
		Window(uint32_t length, uint32_t animationFrameStart, uint32_t animationFrameCount)
			: length(length), animationFrameStart(animationFrameStart), animationFrameCount(animationFrameCount) {}

		Window withLoopType(WindowLoopType loopType) const
		{
			auto out = *this;
			out.loopType = loopType;
			return out;
		}
		Window withMovement(Fixed moveX, Fixed moveY) const
		{
			auto out = *this;
			out.moveX = moveX;
			out.moveY = moveY;
			return out;
		}

		const uint32_t &getLength() const
		{
			return length;
		}
		const uint32_t &getAnimationFrameStart() const
		{
			return animationFrameStart;
		}
		const uint32_t &getAnimationFrameCount() const
		{
			return animationFrameCount;
		}
		const WindowLoopType &getLoopType() const
		{
			return loopType;
		}
		const Fixed &getMoveX() const
		{
			return moveX;
		}
		const Fixed &getMoveY() const
		{
			return moveY;
		}

	private:
		uint32_t length;
		uint32_t animationFrameStart, animationFrameCount;
		WindowLoopType loopType = WindowLoopType::DoNotLoop;
		Fixed moveX, moveY;
	};

	enum class MoveType
	{
		Jab,
		DashAttack,
		ForwardTilt,
		ForwardSmash,
		UpSmash,
		DownSmash
		// TODO add more move types
	};

	struct Move
	{
		const std::vector<Window> windows;
		const std::vector<Hitbox> hitboxes;

		Move(const std::vector<Window> &windows, const std::vector<Hitbox> &hitboxes)
			: windows(windows), hitboxes(hitboxes) {}
	};

	struct Character
	{
		const std::map<MoveType, Move> moves;

		Character(const std::map<MoveType, Move> &moves) : moves(moves) {}
	};
}

#endif //DBKRPLATFORMFIGHTER_INTERFACE_H
