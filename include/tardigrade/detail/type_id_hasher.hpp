#pragma once
#include <tardigrade/detail/type_id.hpp>
#include <functional>

namespace tg {
struct TypeId::Hasher {
	std::size_t operator()(TypeId const& id) const { return std::hash<TypeId::value_type>{}(id.m_id); }
};
} // namespace tg
