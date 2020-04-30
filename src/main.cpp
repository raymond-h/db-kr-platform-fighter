#include <iostream>
#include <fstream>
#include <ggponet.h>
#include <entt/entt.hpp>
#include <SDL.h>

#include "components.hpp"
#include "prefabs.hpp"
#include "aabb.hpp"
#include "fixed.hpp"
#include "snapshot.hpp"

#include "systems/systems.hpp"

using namespace std;

bool handleEvent(entt::registry& registry, int32_t &moveX, int32_t &moveY, SDL_Event &event)
{
	switch (event.type)
	{
	case SDL_QUIT:
		return true;

	case SDL_KEYDOWN:
	{
		if (event.key.repeat)
			break;

		switch (event.key.keysym.scancode)
		{
		case SDL_SCANCODE_LEFT:
			moveX--;
			break;
		case SDL_SCANCODE_RIGHT:
			moveX++;
			break;
		case SDL_SCANCODE_UP:
			moveY--;
			break;
		case SDL_SCANCODE_DOWN:
			moveY++;
			break;
		case SDL_SCANCODE_S:
			auto res = saveSnapshot(registry);
			ofstream fout("out.bin", ios::binary);
			fout << res;
			fout.close();
			break;
		}
		break;
	}

	case SDL_KEYUP:
	{
		if (event.key.repeat)
			break;

		switch (event.key.keysym.scancode)
		{
		case SDL_SCANCODE_LEFT:
			moveX++;
			break;
		case SDL_SCANCODE_RIGHT:
			moveX--;
			break;
		case SDL_SCANCODE_UP:
			moveY++;
			break;
		case SDL_SCANCODE_DOWN:
			moveY--;
			break;
		}
		break;
	}

	default:
		break;
	}

	return false;
}

int main_game(int argc, char *argv[])
{
	entt::registry registry;

	SDL_Init(SDL_INIT_VIDEO);

	shared_ptr<SDL_Window> window(
		SDL_CreateWindow(
			"DB KR Platform Fighter",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			640,
			480,
			0),
		SDL_DestroyWindow);

	if (window == nullptr)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	shared_ptr<SDL_Renderer> renderer(
		SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), SDL_DestroyRenderer);

	if (renderer == nullptr)
	{
		printf("Could not create renderer: %s\n", SDL_GetError());
		window.reset();
		SDL_Quit();
		return 1;
	}

	int32_t moveX = 0, moveY = 0;

	createGround(registry, 120, 300, 60, 150);
	createGround(registry, 520, 300, 60, 150);
	createGround(registry, 320, 240 + 120, 400, 60);

	createPlayer(registry, 320, 240, 16, 16);

	const int frameLength = 1000 / 60;

	// ArchiveExperiment archiveEx;

	// registry.snapshot()
	// 	.entities(archiveEx)
	// 	.destroyed(archiveEx)
	// 	.component<Position, Velocity>(archiveEx);

	bool done = false;
	while (true)
	{
		int frameStartMs = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			done = handleEvent(registry, moveX, moveY, event);
			if (done)
				break;
		}
		if (done)
			break;

		update(moveX, moveY, registry);

		render(renderer, registry);

		int frameEndMs = SDL_GetTicks();

		SDL_Delay(max(0, frameLength - (frameEndMs - frameStartMs)));
	}

	SDL_Quit();

	return 0;
}

int main(int argc, char *argv[])
{
	return main_game(argc, argv);
}