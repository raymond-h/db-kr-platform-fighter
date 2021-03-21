#ifndef __INPUT_UPDATE_HPP__
#define __INPUT_UPDATE_HPP__

#include <variant>
#include <entt/entt.hpp>

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

std::variant<WindowChange, FighterStateEnum> computeNextWindow(
	const FighterState &fs,
	const FighterInput &fighterInput);

void updateChara(const FighterState &fs, const FighterInput &fighterInput, Velocity &vel, facing_t &facing);

inline void setCurrentState(FighterState &fs, FighterStateEnum nextState)
{
	fs.fighterState = nextState;
	fs.currentStateFrameCounter = 0;

	fs.window = 0;
	fs.currentWindowFrameCounter = 0;
}

inline void inputUpdate(const CompleteInputData &completeInputData, entt::registry &registry)
{
	const auto frameCounter = registry.ctx<int64_t>();

	registry.view<PlayerControllable, FighterInput>().each([&completeInputData](auto &pCon, auto &fi) {
		const auto inputData = completeInputData.inputDatas[pCon.playerIndex];
		assignFighterInputs(inputData, fi);
		// std::cout << "[" << frameCounter << "] Player #" << (pCon.playerIndex) << " inputs: " << inputData << std::endl;
	});

	auto players = registry.view<FighterState, FighterInput, Velocity, GroundCollisionFlags>();
	for (auto &entity : players)
	{
		auto [fs, fi, vel, gColFlags] = registry.get<FighterState, FighterInput, Velocity, GroundCollisionFlags>(entity);

		const auto nextState = computeNextState(fs, fi, vel, gColFlags);

		if (nextState != fs.fighterState)
		{
			setCurrentState(fs, nextState);
		}
		else
		{
			fs.currentStateFrameCounter++;

			const auto nextWindowOrState = computeNextWindow(fs, fi);

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

		std::cout << "[" << frameCounter << "] " << fs.fighterState << "; " << fs.facing << "; " << fs.window << "; " << fi.isStrong << std::endl;
		// std::cout << "[" << frameCounter << "] " << fi.moveAngle<4>() << std::endl;

		// update vel from state
		facing_t &facing = fs.facing;
		updateChara(fs, fi, vel, facing);
		fs.facing = facing;
	}
}

#endif // __INPUT_UPDATE_HPP__
