#ifndef __INPUT_UPDATE_HPP__
#define __INPUT_UPDATE_HPP__

#include <entt/entt.hpp>
#include <variant>

#include "../components.hpp"
#include "../input.hpp"

void assignFighterInputs(const InputData &inputData, FighterInput &fi);

FighterStateEnum computeNextState(
	const FighterState &fs,
	const FighterInput &fighterInput,
	const Velocity &vel,
	const GroundCollisionFlags &gColFlags);

FighterStateEnum computeNextStateStandard(
	const FighterState &fs,
	const FighterInput &fighterInput,
	const Velocity &vel,
	const GroundCollisionFlags &gColFlags);

FighterStateEnum computeNextStateAttackMove(
	const FighterState &fs,
	const FighterInput &fighterInput,
	const Velocity &vel,
	const GroundCollisionFlags &gColFlags);

FighterStateEnum computeNextStateEarlyCancel(
	const FighterState &fs,
	const FighterInput &fighterInput,
	const Velocity &vel,
	const GroundCollisionFlags &gColFlags);

struct WindowChange
{
	window_t nextWindow;
};

std::variant<WindowChange, FighterStateEnum>
computeNextWindow(const FighterState &fs, const FighterInput &fighterInput, const FighterData &data);

void updateChara(const FighterState &fs, const FighterInput &fighterInput, Velocity &vel, facing_t &facing,
	const FighterData &data);

inline void setCurrentState(FighterState &fs, const FighterStateEnum nextState)
{
	fs.fighterState = nextState;
	fs.currentStateFrameCounter = 0;

	fs.window = 0;
	fs.currentWindowFrameCounter = 0;
}

inline void inputUpdate(const CompleteInputData &completeInputData, entt::registry &registry)
{
	const auto frameCounter = registry.ctx<int64_t>();

	registry.view<const PlayerControllable, FighterInput>().each(
		[&completeInputData](auto &pCon, auto &fi)
		{
			const auto inputData = completeInputData.inputDatas[pCon.playerIndex];
			assignFighterInputs(inputData, fi);
			// std::cout << "[" << frameCounter << "] Player #" << (pCon.playerIndex) << " inputs: " << inputData << std::endl;
		}
	);

	registry.view<FighterState, const FighterInput, Velocity, const GroundCollisionFlags, const FighterData>().each(
		[](FighterState &fs, const FighterInput &fi, Velocity &vel, const GroundCollisionFlags &gColFlags,
			const FighterData &fighterData)
		{
			const auto nextState = computeNextState(fs, fi, vel, gColFlags);

			if (nextState != fs.fighterState)
			{
				setCurrentState(fs, nextState);
			}
			else
			{
				fs.currentStateFrameCounter++;

				const auto nextWindowOrState = computeNextWindow(fs, fi, fighterData);

				if (std::holds_alternative<WindowChange>(nextWindowOrState))
				{
					const auto nextWindow = std::get<WindowChange>(nextWindowOrState).nextWindow;
					if (fs.window != nextWindow)
					{
						fs.window = nextWindow;
						fs.currentWindowFrameCounter = 0;
					}
					else
					{
						fs.currentWindowFrameCounter++;
					}
				}
				else
				{
					const auto nextState = std::get<FighterStateEnum>(nextWindowOrState);
					assert(nextState != fs.fighterState);
					setCurrentState(fs, nextState);
				}
			}

			// std::cout << "[" << frameCounter << "] " << fs.fighterState << "; " << fs.facing << "; " << fs.window << "; " << fi.isStrong << std::endl;
			// std::cout << "[" << frameCounter << "] " << fi.moveAngle<4>() << std::endl;

			// update vel from state
			auto facing = fs.facing;
			updateChara(fs, fi, vel, facing, fighterData);
			fs.facing = facing;
		}
	);
}

#endif// __INPUT_UPDATE_HPP__
