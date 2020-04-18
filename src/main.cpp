#include <iostream>
#include <ggponet.h>
#include <entt/entt.hpp>
#include <SDL.h>

using namespace std;

struct PlayerControllable
{
};

struct Position
{
	int32_t x;
	int32_t y;
};

struct Velocity
{
	int32_t x;
	int32_t y;
};

bool handleEvent(int32_t &moveX, int32_t &moveY, SDL_Event &event)
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

void inputUpdate(int32_t moveX, int32_t moveY, entt::registry &registry)
{
	auto players = registry.view<PlayerControllable, Velocity>();
	for (auto &entity : players)
	{
		auto &vel = registry.get<Velocity>(entity);

		vel.x = moveX;
		vel.y = moveY;
	}
}

void positionUpdate(entt::registry &registry)
{
	registry.view<Position, Velocity>().each([](Position &pos, Velocity &vel) {
		pos.x += vel.x;
		pos.y += vel.y;
	});
}

void update(int32_t moveX, int32_t moveY, entt::registry &registry)
{
	inputUpdate(moveX, moveY, registry);
	positionUpdate(registry);
}

void render(shared_ptr<SDL_Renderer> renderer, entt::registry &registry)
{
	SDL_Renderer* rendererP = renderer.get();

	SDL_SetRenderDrawColor(rendererP, 0, 0, 0, 255);
	SDL_RenderClear(rendererP);

	registry.view<Position>().each([rendererP](Position &pos) {
		SDL_Rect rect;
		rect.x = pos.x - 8;
		rect.y = pos.y - 8;
		rect.w = 16;
		rect.h = 16;
		SDL_SetRenderDrawColor(rendererP, 255, 0, 0, 255);
		SDL_RenderFillRect(rendererP, &rect);
	});

	SDL_RenderPresent(rendererP);
}

int main(int argc, char *argv[])
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

	auto playerEntity = registry.create();
	registry.emplace<PlayerControllable>(playerEntity);
	registry.emplace<Position>(playerEntity, 320, 240);
	registry.emplace<Velocity>(playerEntity, 0, 0);

	const int frameLength = 1000 / 60;

	bool done = false;
	while (true)
	{
		int frameStartMs = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			done = handleEvent(moveX, moveY, event);
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