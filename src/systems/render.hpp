#ifndef __RENDER_HPP__
#define __RENDER_HPP__

#include <entt/entt.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "components.hpp"

inline void render(GPU_Target &screen, entt::registry &registry)
{
	ImGui::Render();

	GPU_ClearRGBA(&screen, 0, 0, 0, 255);

	registry.view<Position, CollisionBox>().each(
		[&screen](Position &pos, CollisionBox &cbox)
		{
			SDL_Rect rect;
			rect.x = int(pos.x - cbox.width / 2);
			rect.y = int(pos.y - cbox.height / 2);
			rect.w = int(cbox.width);
			rect.h = int(cbox.height);
			SDL_Color rectColor{255, 0, 0, 255};
			GPU_RectangleFilled(
				&screen,
				float(rect.x),
				float(rect.y),
				float(rect.x + rect.w),
				float(rect.y + rect.h),
				rectColor
			);
		}
	);

	registry.view<Position, GroundCollisionFlags>().each(
		[&screen](Position &pos, GroundCollisionFlags &gcolflags)
		{
			const float lineLen = 15.f;

			float x = (float)pos.x, y = (float)pos.y;

			SDL_Color lineColor{0, 255, 0, 255};
			if (gcolflags.left)
			{
				GPU_Line(&screen, x, y, x - lineLen, y, lineColor);
			}
			if (gcolflags.right)
			{
				GPU_Line(&screen, x, y, x + lineLen, y, lineColor);
			}
			if (gcolflags.top)
			{
				GPU_Line(&screen, x, y, x, y - lineLen, lineColor);
			}
			if (gcolflags.bottom)
			{
				GPU_Line(&screen, x, y, x, y + lineLen, lineColor);
			}
		}
	);

	GPU_FlushBlitBuffer();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	GPU_Flip(&screen);
}

#endif// __RENDER_HPP__
