#pragma once
#include <tardigrade/detail/type_id_hasher.hpp>
#include <cassert>
#include <concepts>
#include <unordered_map>

namespace tg {
template <typename T>
concept Service = std::is_pointer_v<T>;

class Services {
  public:
	class Scoped;

	template <Service Type>
	static void provide(Type service) {
		provide(service, TypeId::make<Type>());
	}

	template <Service Type>
	static Type find() {
		if (auto it = s_map.find(TypeId::make<Type>()); it != s_map.end()) { return static_cast<Type>(it->second); }
		return {};
	}

	template <Service Type>
	static bool contains() {
		return find<Type>() != nullptr;
	}

	template <Service Type>
	static void unprovide() {
		s_map.erase(TypeId::make<Type>());
	}

	static std::size_t count() { return s_map.size(); }

  private:
	static void provide(void const* t, TypeId type) { s_map[type] = const_cast<void*>(t); }
	static void provide(void* t, TypeId type) { s_map[type] = t; }

	inline static std::unordered_map<TypeId, void*, TypeId::Hasher> s_map{};
};

template <typename Type>
class ServiceProvider : public Type {
  public:
	template <typename... Args>
	ServiceProvider(Args&&... args) noexcept : Type(std::forward<Args>(args)...) {
		Services::provide<Type*>(this);
	}

	~ServiceProvider() noexcept { Services::unprovide<Type*>(); }

	ServiceProvider& operator=(ServiceProvider&&) = delete;
};

class Services::Scoped {
  public:
	Scoped() = default;
	~Scoped() noexcept { s_map.clear(); }

	Scoped& operator=(Scoped&&) = delete;
};

template <Service Type>
Type locate() {
	auto ret = Services::find<Type>();
	assert(ret && "Requested Service not found");
	return ret;
}
} // namespace tg
