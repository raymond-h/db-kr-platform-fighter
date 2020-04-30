#ifndef __RENDER_HPP__
#define __RENDER_HPP__

#include <entt/entt.hpp>

#include "components.hpp"

inline void render(std::shared_ptr<SDL_Renderer> renderer, entt::registry &registry)
{
    SDL_Renderer *rendererP = renderer.get();

    SDL_SetRenderDrawColor(rendererP, 0, 0, 0, 255);
    SDL_RenderClear(rendererP);

    registry.view<Position, CollisionBox>().each([rendererP](Position &pos, CollisionBox &cbox) {
        SDL_Rect rect;
        rect.x = int(pos.x - cbox.width / 2);
        rect.y = int(pos.y - cbox.height / 2);
        rect.w = int(cbox.width);
        rect.h = int(cbox.height);
        SDL_SetRenderDrawColor(rendererP, 255, 0, 0, 255);
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

    SDL_RenderPresent(rendererP);
}

#endif // __RENDER_HPP__
