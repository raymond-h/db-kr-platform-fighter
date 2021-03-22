#ifndef __IMGUI_DEBUG_HPP__
#define __IMGUI_DEBUG_HPP__

#include "fighter_state.hpp"
#include <entt/entt.hpp>
#include <imgui.h>

inline void drawDebugWindow(entt::registry &registry)
{
    ImGui::Begin("Debug");

    auto players = registry.view<FighterState, FighterInput, Velocity, GroundCollisionFlags>();
    for (auto &entity : players)
    {
        auto [fs, fi, vel, gColFlags] = registry.get<FighterState, FighterInput, Velocity, GroundCollisionFlags>(entity);

        ImGui::Text("State: %s", fighter_state_enum_to_string(fs.fighterState));
        ImGui::Text("Facing: %s", fs.facing < 0 ? "Left" : "Right");
        ImGui::Text("Window: %d", fs.window);
        ImGui::Text("Was stick strong input? %s", fi.isStrong ? "Yes" : "No");
    }

    ImGui::End();
}

#endif // __IMGUI_DEBUG_HPP__