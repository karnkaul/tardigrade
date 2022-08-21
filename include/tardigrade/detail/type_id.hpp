#pragma once
#include <cstdint>
#include <type_traits>

namespace tg {
class TypeId {
  public:
	template <typename T>
	static TypeId make() {
		return get<std::remove_cvref_t<T>>();
	}

	bool operator==(TypeId const&) const = default;

	struct Hasher;

  private:
	using value_type = std::uint64_t;

	template <typename T>
	static value_type get() {
		static auto const id = next();
		return id;
	}

	static value_type next() {
		static auto s_next = value_type{};
		return ++s_next;
	}

	TypeId(value_type id) noexcept : m_id(id) {}

	value_type m_id{};
};
} // namespace tg
