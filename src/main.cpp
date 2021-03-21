#include <iostream>
#include <ggponet.h>
#include <entt/entt.hpp>
#include <SDL.h>
#include <SDL_gpu.h>

#include "components.hpp"
#include "prefabs.hpp"
#include "aabb.hpp"
#include "fixed.hpp"

#include "systems/systems.hpp"

using namespace std;

bool handleEvent(InputData &inputData, SDL_Event &event)
{
	switch (event.type)
	{
	case SDL_QUIT:
		return true;

	case SDL_CONTROLLERAXISMOTION:
		if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
		{
			inputData.moveX = event.caxis.value;
		}
		else if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
		{
			inputData.moveY = event.caxis.value;
		}
		break;

	case SDL_CONTROLLERBUTTONDOWN:
		if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
		{
			inputData.doJump = true;
		}
		else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
		{
			inputData.doNormalAttack = true;
		}
		break;

	case SDL_CONTROLLERBUTTONUP:
		if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
		{
			inputData.doJump = false;
		}
		else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
		{
			inputData.doNormalAttack = false;
		}
		break;

		// case SDL_KEYDOWN:
		// {
		// 	if (event.key.repeat)
		// 		break;

		// 	switch (event.key.keysym.scancode)
		// 	{
		// 	case SDL_SCANCODE_LEFT:
		// 		inputData.moveX -= 1;
		// 		break;
		// 	case SDL_SCANCODE_RIGHT:
		// 		inputData.moveX += 1;
		// 		break;
		// 	case SDL_SCANCODE_UP:
		// 		inputData.moveY -= 1;
		// 		break;
		// 	case SDL_SCANCODE_DOWN:
		// 		inputData.moveY += 1;
		// 		break;
		// 	}
		// 	break;
		// }

		// case SDL_KEYUP:
		// {
		// 	if (event.key.repeat)
		// 		break;

		// 	switch (event.key.keysym.scancode)
		// 	{
		// 	case SDL_SCANCODE_LEFT:
		// 		inputData.moveX += 1;
		// 		break;
		// 	case SDL_SCANCODE_RIGHT:
		// 		inputData.moveX -= 1;
		// 		break;
		// 	case SDL_SCANCODE_UP:
		// 		inputData.moveY += 1;
		// 		break;
		// 	case SDL_SCANCODE_DOWN:
		// 		inputData.moveY -= 1;
		// 		break;
		// 	}
		// 	break;
		// }

	default:
		break;
	}

	return false;
}

int main_game(int argc, char *argv[])
{
	entt::registry registry;

	auto screen = GPU_Init(640, 480, GPU_DEFAULT_INIT_FLAGS);
	if (screen == nullptr)
	{
		auto code = GPU_PopErrorCode();
		printf("Could not initialize SDL_gpu: %s: %s\n", GPU_GetErrorString(code.error), code.details);
		return 1;
	}

	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

	cout << "Joystick count: " << SDL_NumJoysticks() << endl;
	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		if (SDL_IsGameController(i))
		{
			cout << "Index \'" << i << "\' is a compatible controller, named \'" << SDL_GameControllerNameForIndex(i) << "\'" << endl;
			auto ctrl = SDL_GameControllerOpen(i);
		}
		else
		{
			cout << "Index \'" << i << "\' is not a compatible controller." << endl;
		}
	}

	CompleteInputData completeInputData{
		{0, 0, false, false, 0, 0}};

	createGround(registry, 120, 300, 60, 150);
	createGround(registry, 520, 300, 60, 150);
	createGround(registry, 320, 240 + 120, 400, 60);

	createPlayer(registry, 0, 320, 240, 16, 16);

	const int frameLength = 1000 / 60;

	bool done = false;
	int64_t frameCounter = 0;
	while (true)
	{
		registry.set<int64_t>(frameCounter);

		int frameStartMs = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			done = handleEvent(completeInputData.inputDatas[0], event);
			if (done)
				break;
		}
		if (done)
			break;

		for (auto &&id : completeInputData.inputDatas)
		{
			id.updateFrameCounters(frameCounter);
		}

		update(completeInputData, registry);

		render(*screen, registry);

		int frameEndMs = SDL_GetTicks();

		SDL_Delay(max(0, frameLength - (frameEndMs - frameStartMs)));
		++frameCounter;
	}

	GPU_Quit();

	return 0;
}

int main(int argc, char *argv[])
{
	return main_game(argc, argv);
}