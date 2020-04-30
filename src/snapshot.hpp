#ifndef __SNAPSHOT_HPP__
#define __SNAPSHOT_HPP__

#include <sstream>
#include <entt/entt.hpp>
#include <cereal/archives/binary.hpp>

#include "components.hpp"

using NetSyncComponent = NetSynced;

auto saveSnapshot(entt::registry &registry)
{
    std::stringstream data(std::ios::binary | std::ios::out);
    cereal::BinaryOutputArchive outArchive(data);

    auto view = registry.view<NetSyncComponent>();

    registry.snapshot()
        // .entities(outArchive)
        // .destroyed(outArchive)
        .component<SNAPSHOT_COMPONENTS>(outArchive, view.begin(), view.end());

    return data.str();
}

void loadSnapshot(entt::registry &registry, const std::string &str)
{
    std::stringstream data(str, std::ios::binary | std::ios::in);
    cereal::BinaryInputArchive inArchive(data);
    entt::continuous_loader loader{registry};

    auto view = registry.view<NetSyncComponent>();
    registry.destroy(view.begin(), view.end());

    loader
        // .entities(inArchive)
        // .destroyed(inArchive)
        .component<SNAPSHOT_COMPONENTS>(inArchive)
        .orphans()
        .shrink();
}

#endif // __SNAPSHOT_HPP__
