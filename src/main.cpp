#include <iostream>
#include <ggponet.h>
#include <entt/entt.hpp>
#include <SDL.h>

#include "components.hpp"
#include "aabb.hpp"
#include "fixed.hpp"

using namespace std;

entt::entity createHitbox(
	entt::registry &registry,
	entt::entity ownerEntity,
	coord_t offsetX, coord_t offsetY,
	coord_t w, coord_t h,
	coord_t impulseX, coord_t impulseY,
	int32_t lifetime);

AABB boxForEntity(Position &pos, CollisionBox &cbox)
{
	return {
		pos.x - cbox.width / 2, pos.y - cbox.height / 2,
		pos.x + cbox.width / 2, pos.y + cbox.height / 2};
}

bool handleEvent(int32_t &moveX, int32_t &moveY, bool &normalAttack, SDL_Event &event)
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

		case SDL_SCANCODE_Z:
			normalAttack = true;
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

void inputUpdate(coord_t moveX, coord_t moveY, bool normalAttack, entt::registry &registry)
{
	auto movablePlayers = registry.view<PlayerControllable, Velocity, GroundCollisionFlags>();
	for (auto &entity : movablePlayers)
	{
		auto [vel, gColFlags] = registry.get<Velocity, GroundCollisionFlags>(entity);
		auto facing = registry.try_get<Facing>(entity);

		if (facing != nullptr && moveX != 0)
		{
			facing->facingRight = moveX > 0;
		}

		vel.x = moveX * 5 / 2;
		if (gColFlags.bottom && moveY < 0)
		{
			vel.y = -5;
		}
	}

	auto hittingPlayers = registry.view<PlayerControllable, Position, Facing>();
	for (auto &entity : hittingPlayers)
	{
		auto facing = registry.get<Facing>(entity);

		if (normalAttack)
		{
			auto facingFactor = facing.facingRight ? 1 : -1;
			createHitbox(registry, entity, facingFactor * 14, 0, 8, 8, facingFactor * 3, -3, 3);
		}
	}
}

void positionUpdate(entt::registry &registry)
{
	registry.view<Position, Velocity, GroundCollisionFlags>().each([](Position &pos, Velocity &vel, GroundCollisionFlags &gColFlags) {
		if (gColFlags.bottom && vel.y > 0)
		{
			vel.y = 0;
		}

		vel.y += Fixed(1) / 10;
		vel.x *= Fixed(95) / 100;

		pos.x += vel.x;
		pos.y += vel.y;
	});
}

void resetCollisionFlags(entt::registry &registry)
{
	registry.view<GroundCollisionFlags>().each([](GroundCollisionFlags &groundCollisionFlags) {
		groundCollisionFlags.left = groundCollisionFlags.top =
			groundCollisionFlags.right = groundCollisionFlags.bottom = false;
	});
}

void handleGroundCollision(entt::registry &registry, entt::entity ground, entt::entity other)
{
	auto [apos, acbox] = registry.get<Position, CollisionBox>(ground);
	auto [bpos, bcbox] = registry.get<Position, CollisionBox>(other);
	auto bGColFlags = registry.try_get<GroundCollisionFlags>(other);

	auto aAabb = boxForEntity(apos, acbox);
	auto bAabb = boxForEntity(bpos, bcbox);

	coord_t overlapX = 0, overlapY = 0;
	calculateOverlap(aAabb, bAabb, overlapX, overlapY);

	if (abs(overlapX) < abs(overlapY))
	{
		bpos.x += overlapX;
		if (overlapX != 0 && bGColFlags != nullptr)
		{
			if (overlapX < 0)
			{
				bGColFlags->right = true;
			}
			else
			{
				bGColFlags->left = true;
			}
		}
	}
	else
	{
		bpos.y += overlapY;
		if (overlapY != 0 && bGColFlags != nullptr)
		{
			if (overlapY < 0)
			{
				bGColFlags->bottom = true;
			}
			else
			{
				bGColFlags->top = true;
			}
		}
	}
}

void handleHitboxCollision(entt::registry &registry, entt::entity hitboxEnt, entt::entity other)
{
	auto [apos, acbox, aHitbox] = registry.get<Position, CollisionBox, Hitbox>(hitboxEnt);
	if (aHitbox.owner == other)
		return;

	auto [bpos, bcbox, bVel] = registry.get<Position, CollisionBox, Velocity>(other);

	auto aAabb = boxForEntity(apos, acbox);
	auto bAabb = boxForEntity(bpos, bcbox);

	if (isOverlap(aAabb, bAabb))
	{
		bVel.x = aHitbox.impulseX;
		bVel.y = aHitbox.impulseY;
	}
}

void collisionUpdate(entt::registry &registry)
{
	auto ents = registry.view<Position, CollisionBox>();
	auto entsEnd = ents.end();
	for (auto it = ents.begin(); it != entsEnd; it++)
	{
		auto itInner = it;
		itInner++;
		for (; itInner != entsEnd; itInner++)
		{
			auto a = *it;
			auto b = *itInner;

			auto aIsGround = registry.has<Ground>(a);
			auto bIsGround = registry.has<Ground>(b);

			auto aIsHitbox = registry.has<Hitbox>(a);
			auto bIsHitbox = registry.has<Hitbox>(b);

			assert(!(aIsGround && aIsHitbox));
			assert(!(bIsGround && bIsHitbox));

			auto aIsPlain = !(aIsGround || aIsHitbox);
			auto bIsPlain = !(bIsGround || bIsHitbox);

			if (aIsGround && bIsPlain)
			{
				handleGroundCollision(registry, a, b);
			}
			else if (aIsPlain && bIsGround)
			{
				handleGroundCollision(registry, b, a);
			}
			else if (aIsHitbox && bIsPlain)
			{
				handleHitboxCollision(registry, a, b);
			}
			else if (aIsPlain && bIsHitbox)
			{
				handleHitboxCollision(registry, b, a);
			}
		}
	}
}

void updateHitboxPosition(entt::registry &registry)
{
	registry.view<Position, Hitbox>().each([&](Position &pos, Hitbox &hitbox) {
		auto ownerPos = registry.get<Position>(hitbox.owner);

		pos.x = ownerPos.x + hitbox.ownerOffsetX;
		pos.y = ownerPos.y + hitbox.ownerOffsetY;
	});
}

void updateRemoveAfterLifetime(entt::registry &registry)
{
	registry.view<RemoveAfterLifetime>().each([&](const auto ent, RemoveAfterLifetime &rAL) {
		if (rAL.lifetime-- <= 0)
		{
			registry.destroy(ent);
		}
	});
}

void update(coord_t moveX, coord_t moveY, bool normalAttack, entt::registry &registry)
{
	inputUpdate(moveX, moveY, normalAttack, registry);
	positionUpdate(registry);
	resetCollisionFlags(registry);
	collisionUpdate(registry);
	updateHitboxPosition(registry);
	updateRemoveAfterLifetime(registry);
}

void render(shared_ptr<SDL_Renderer> renderer, entt::registry &registry)
{
	SDL_Renderer *rendererP = renderer.get();

	SDL_SetRenderDrawColor(rendererP, 0, 0, 0, 255);
	SDL_RenderClear(rendererP);

	registry.view<Position, CollisionBox>().each([&registry, rendererP](entt::entity ent, Position &pos, CollisionBox &cbox) {
		SDL_Rect rect;
		rect.x = int(pos.x - cbox.width / 2);
		rect.y = int(pos.y - cbox.height / 2);
		rect.w = int(cbox.width);
		rect.h = int(cbox.height);
		if (registry.has<Hitbox>(ent))
		{
			SDL_SetRenderDrawColor(rendererP, 127, 127, 255, 255);
		}
		else
		{
			SDL_SetRenderDrawColor(rendererP, 255, 0, 0, 255);
		}
		SDL_RenderFillRect(rendererP, &rect);
	});

	registry.view<Position, GroundCollisionFlags>().each([rendererP](Position &pos, GroundCollisionFlags &gcolflags) {
		const int lineLen = 15;

		int32_t x = (int32_t)pos.x, y = (int32_t)pos.y;

		SDL_SetRenderDrawColor(rendererP, 0, 255, 0, 255);
		if (gcolflags.left)
		{
			SDL_RenderDrawLine(rendererP, x, y, x - lineLen, y);
		}
		if (gcolflags.right)
		{
			SDL_RenderDrawLine(rendererP, x, y, x + lineLen, y);
		}
		if (gcolflags.top)
		{
			SDL_RenderDrawLine(rendererP, x, y, x, y - lineLen);
		}
		if (gcolflags.bottom)
		{
			SDL_RenderDrawLine(rendererP, x, y, x, y + lineLen);
		}
	});

	registry.view<Position, Facing>().each([rendererP](Position &pos, Facing &facing) {
		const int lineLen = 15;

		int32_t x = (int32_t)pos.x, y = (int32_t)pos.y;

		SDL_SetRenderDrawColor(rendererP, 255, 255, 255, 255);
		if (facing.facingRight)
		{
			SDL_RenderDrawLine(rendererP, x, y - 5, x + lineLen, y - 5);
		}
		else
		{
			SDL_RenderDrawLine(rendererP, x, y - 5, x - lineLen, y - 5);
		}
	});

	SDL_RenderPresent(rendererP);
}

auto createPlayer(entt::registry &registry, coord_t x, coord_t y, coord_t w, coord_t h)
{
	auto playerEntity = registry.create();
	registry.emplace<PlayerControllable>(playerEntity);
	registry.emplace<Position>(playerEntity, x, y);
	registry.emplace<CollisionBox>(playerEntity, w, h);
	registry.emplace<Velocity>(playerEntity, 0, 0);
	registry.emplace<GroundCollisionFlags>(playerEntity, false, false, false, false);
	registry.emplace<Facing>(playerEntity, true);
	return playerEntity;
}

auto createNPC(entt::registry &registry, coord_t x, coord_t y, coord_t w, coord_t h)
{
	auto playerEntity = registry.create();
	registry.emplace<Position>(playerEntity, x, y);
	registry.emplace<CollisionBox>(playerEntity, w, h);
	registry.emplace<Velocity>(playerEntity, 0, 0);
	registry.emplace<GroundCollisionFlags>(playerEntity, false, false, false, false);
	registry.emplace<Facing>(playerEntity, true);
	// registry.emplace<RemoveAfterLifetime>(playerEntity, 600);
	return playerEntity;
}

auto createGround(entt::registry &registry, coord_t x, coord_t y, coord_t w, coord_t h)
{
	auto ent = registry.create();
	registry.emplace<Position>(ent, x, y);
	registry.emplace<CollisionBox>(ent, w, h);
	registry.emplace<Ground>(ent);
	return ent;
}

entt::entity createHitbox(
	entt::registry &registry,
	entt::entity ownerEntity,
	coord_t offsetX, coord_t offsetY,
	coord_t w, coord_t h,
	coord_t impulseX, coord_t impulseY,
	int32_t lifetime)
{
	auto pos = registry.get<Position>(ownerEntity);

	auto ent = registry.create();
	registry.emplace<Position>(ent, pos.x + offsetX, pos.y + offsetY);
	registry.emplace<CollisionBox>(ent, w, h);
	registry.emplace<Hitbox>(ent, ownerEntity, offsetX, offsetY, impulseX, impulseY);
	registry.emplace<RemoveAfterLifetime>(ent, lifetime);
	return ent;
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

	// inputs
	int32_t moveX = 0, moveY = 0;
	bool normalAttack = false;

	createGround(registry, 120, 300, 60, 150);
	createGround(registry, 520, 300, 60, 150);
	createGround(registry, 320, 240 + 120, 400, 60);

	createPlayer(registry, 320, 240, 16, 16);
	createNPC(registry, 400, 240, 16, 16);

	const int frameLength = 1000 / 60;

	bool done = false;
	while (true)
	{
		int frameStartMs = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			done = handleEvent(moveX, moveY, normalAttack, event);
			if (done)
				break;
		}
		if (done)
			break;

		update(moveX, moveY, normalAttack, registry);

		normalAttack = false;

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