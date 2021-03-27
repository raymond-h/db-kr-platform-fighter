#include "experiment.h"

namespace Data::Characters
{
	Character getExperimentCharacterData()
	{
		const Move jab(
			{
				Window(3, 0, 3),
				Window(4, 3, 4),
				Window(3, 7, 3)
			}, {
				Hitbox(1, 4)
			}
		);

		const Move forwardTilt(
			{
				Window(10, 0, 10),
				Window(5, 10, 5),
				Window(10, 15, 10)
			}, {
				Hitbox(1, 5)
			}
		);

		const Move dashAttack(
			{
				Window(7, 0, 3).withMovement(Fixed(10) / 2, 0),
				Window(5, 3, 5).withMovement(Fixed(4) / 2, 0)
			}, {
				Hitbox(0, 50)
			}
		);

		const Move forwardSmash(
			{
				Window(20, 0, 20),
				Window(5, 20, 5).withLoopType(WindowLoopType::LoopUntilButtonLetGo),
				Window(40, 25, 40)
			}, {
				Hitbox(2, 20)
			}
		);

		const Move upSmash(
			{
				Window(20, 0, 20),
				Window(5, 20, 5).withLoopType(WindowLoopType::LoopUntilButtonLetGo),
				Window(40, 25, 40)
			}, {
				Hitbox(2, 20)
			}
		);

		const Move downSmash(
			{
				Window(20, 0, 20),
				Window(5, 20, 5).withLoopType(WindowLoopType::LoopUntilButtonLetGo),
				Window(40, 25, 40)
			}, {
				Hitbox(2, 20)
			}
		);

		return Character(
			{
				{MoveType::Jab, jab},
				{MoveType::ForwardTilt, forwardTilt},
				{MoveType::DashAttack, dashAttack},
				{MoveType::ForwardSmash, forwardSmash},
				{MoveType::UpSmash, upSmash},
				{MoveType::DownSmash, downSmash}
			}
		);
	}
}
