#pragma once
#include <tardigrade/scene.hpp>
#include <string>

namespace tg {
constexpr auto to_underlying(Entity::Id id) { return std::underlying_type_t<Entity::Id>(id); }
inline std::string to_string(Entity::Id id) { return std::to_string(to_underlying(id)); }
} // namespace tg
