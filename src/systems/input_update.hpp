#ifndef __INPUT_UPDATE_HPP__
#define __INPUT_UPDATE_HPP__

#include <entt/entt.hpp>

#include "../components.hpp"
#include "../input.hpp"

void assignFighterInputs(const InputData &inputData, FighterInput &fi);

FighterStateEnum computeNextState(
    const FighterState &fs,
    const FighterInput &fighterInput,
    const Velocity &vel,
    const GroundCollisionFlags &gColFlags);

FighterStateEnum computeNextStateEarlyCancel(
    const FighterState &fs,
    const FighterInput &fighterInput,
    const Velocity &vel,
    const GroundCollisionFlags &gColFlags);

void updateChara(const FighterState &fs, const FighterInput &fighterInput, Velocity &vel);

inline void inputUpdate(const CompleteInputData &completeInputData, entt::registry &registry)
{
	const auto frameCounter = registry.ctx<int64_t>();

	registry.view<PlayerControllable, FighterInput>().each([&completeInputData](auto &pCon, auto &fi) {
		const auto inputData = completeInputData.inputDatas[pCon.playerIndex];
		assignFighterInputs(inputData, fi);
	});

	auto players = registry.view<FighterState, FighterInput, Velocity, GroundCollisionFlags>();
	for (auto &entity : players)
	{
		auto [fs, fi, vel, gColFlags] = registry.get<FighterState, FighterInput, Velocity, GroundCollisionFlags>(entity);

		auto nextState = computeNextStateEarlyCancel(fs, fi, vel, gColFlags);

		if (nextState != fs.fighterState)
		{
			fs.fighterState = nextState;
			fs.currentStateFrameCounter = 0;
		}
		else
		{
			fs.currentStateFrameCounter++;
		}

		std::cout << "[" << frameCounter << "] " << fs.fighterState << std::endl;

		// update vel from state
		updateChara(fs, fi, vel);
	}
}

#endif // __INPUT_UPDATE_HPP__
