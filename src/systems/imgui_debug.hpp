#ifndef __IMGUI_DEBUG_HPP__
#define __IMGUI_DEBUG_HPP__

#include "fighter_state.hpp"
#include <entt/entt.hpp>
#include <imgui.h>

inline void drawDebugWindow(entt::registry &registry)
{
	ImGui::Begin("Debug");

	registry.view<FighterState, FighterInput, Velocity, GroundCollisionFlags>().each(
		[](FighterState fs, FighterInput fi, Velocity vel, GroundCollisionFlags gColFlags)
		{
			ImGui::Text(
				"State: %s (frame: %lld)",
				fighter_state_enum_to_string(fs.fighterState),
				fs.currentStateFrameCounter
			);
			ImGui::Text("Window: %d (frame: %lld)", fs.window, fs.currentWindowFrameCounter);
			ImGui::Text("Facing: %s", fs.facing < 0 ? "Left" : "Right");
			ImGui::Text("Was stick strong input? %s", fi.isStrong ? "Yes" : "No");
		}
	);

	ImGui::End();
}

#endif// __IMGUI_DEBUG_HPP__
