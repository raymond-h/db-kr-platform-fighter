#include <SDL.h>
#include <SDL_gpu.h>
#include <entt/entt.hpp>
#include <ggponet.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <iostream>

#include "aabb.hpp"
#include "components.hpp"
#include "fixed.hpp"
#include "prefabs.hpp"

#include "systems/systems.hpp"

using namespace std;

bool handleEvent(InputData &inputData, bool &showDemoWindow, bool &showDebugWindow, SDL_Event &event)
{
	ImGuiIO &io = ImGui::GetIO();

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

	case SDL_KEYDOWN:
	{
		if (io.WantCaptureKeyboard)
			break;

		if (event.key.repeat)
			break;

		switch (event.key.keysym.scancode)
		{
		case SDL_SCANCODE_F12:
			showDebugWindow = !showDebugWindow;
			break;

		case SDL_SCANCODE_F11:
			showDemoWindow = !showDemoWindow;
			break;

			// case SDL_SCANCODE_LEFT:
			// 	inputData.moveX -= 1;
			// 	break;
			// case SDL_SCANCODE_RIGHT:
			// 	inputData.moveX += 1;
			// 	break;
			// case SDL_SCANCODE_UP:
			// 	inputData.moveY -= 1;
			// 	break;
			// case SDL_SCANCODE_DOWN:
			// 	inputData.moveY += 1;
			// 	break;
		}
		break;
	}

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

const char *glsl_version = "#version 150";

int main_game(int argc, char *argv[])
{
	entt::registry registry;

	GPU_RendererID desiredGpuRendererId = GPU_GetRendererID(GPU_RENDERER_OPENGL_3);
	GPU_SetRendererOrder(1, &desiredGpuRendererId);

	auto screen = GPU_Init(640, 480, GPU_DEFAULT_INIT_FLAGS);
	if (screen == nullptr)
	{
		auto code = GPU_PopErrorCode();
		printf("Could not initialize SDL_gpu: %s: %s\n", GPU_GetErrorString(code.error), code.details);
		return 1;
	}

	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	SDL_Window *window = SDL_GetWindowFromID(screen->context->windowID);
	ImGui_ImplSDL2_InitForOpenGL(window, screen->context->context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	cout << "Joystick count: " << SDL_NumJoysticks() << endl;
	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		if (SDL_IsGameController(i))
		{
			cout << "Index \'" << i << "\' is a compatible controller, named \'" << SDL_GameControllerNameForIndex(i)
				<< "\'" << endl;
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

	bool showDemoWindow = false;
	bool showDebugWindow = false;

	bool done = false;
	int64_t frameCounter = 0;
	while (true)
	{
		registry.set<int64_t>(frameCounter);

		int frameStartMs = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);

			done = handleEvent(completeInputData.inputDatas[0], showDemoWindow, showDebugWindow, event);
			if (done)
				break;
		}
		if (done)
			break;

		for (auto &&id : completeInputData.inputDatas)
		{
			id.updateFrameCounters(frameCounter);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		if (showDemoWindow)
		{
			ImGui::ShowDemoWindow(&showDemoWindow);
		}

		if (showDebugWindow)
		{
			drawDebugWindow(registry);
		}

		update(completeInputData, registry);

		render(*screen, registry);

		int frameEndMs = SDL_GetTicks();

		SDL_Delay(max(0, frameLength - (frameEndMs - frameStartMs)));
		++frameCounter;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	GPU_Quit();

	return 0;
}

int main(int argc, char *argv[])
{
	return main_game(argc, argv);
}