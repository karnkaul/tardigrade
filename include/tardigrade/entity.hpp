#pragma once
#include <tardigrade/detail/ptr.hpp>
#include <tardigrade/detail/type_id.hpp>
#include <memory>

namespace tg {
class Attachment;
class TickAttachment;
class RenderAttachment;
class Scene;
struct RenderList;
struct DeltaTime;

class Entity final {
  public:
	enum struct Id : std::uint64_t { eDefault = 0 };

	Entity();
	Entity(Entity&&) noexcept;
	Entity& operator=(Entity&&) noexcept;
	~Entity() noexcept;

	Entity(Entity const&) = delete;
	Entity& operator=(Entity const&) = delete;

	Entity(Ptr<Scene> scene, Id id);

	Id id() const;

	Entity& set_active(bool active);
	Entity& destroy();

	bool active() const;
	bool destroyed() const;

	template <std::derived_from<Attachment> Type, typename... Args>
	Ptr<Type> attach(Args&&... args) {
		auto t = std::make_unique<Type>(std::forward<Args>(args)...);
		auto ret = t.get();
		attach(std::move(t), TypeId::make<Type>(), make_sub_type<Type>());
		return ret;
	}

	template <std::derived_from<Attachment> Type, typename... Args>
	Ptr<Type> find_or_attach(Args&&... args) {
		if (auto ret = find<Type>()) { return ret; }
		return attach<Type>(std::forward<Args>(args)...);
	}

	template <std::derived_from<Attachment> Type>
	bool contains() const {
		return find<Type>() != nullptr;
	}

	template <std::derived_from<Attachment> Type>
	Ptr<Type> find() const {
		return static_cast<Type*>(find(TypeId::make<Type>()));
	}

	template <std::derived_from<Attachment> Type>
	bool detach() {
		return detach(TypeId::make<Type>());
	}

	std::size_t attached_count() const;
	void clear_attached();

	void tick(DeltaTime dt);

	void fill_render_list(RenderList& out) const;

	Ptr<Scene> scene() const;

	bool sort_ticks{true};

  private:
	enum SubType { eTick = 1 << 0, eRender = 1 << 1 };

	template <typename T>
	static constexpr std::uint8_t make_sub_type() {
		auto ret = std::uint8_t{};
		if constexpr (std::derived_from<T, TickAttachment>) { ret |= eTick; }
		if constexpr (std::derived_from<T, RenderAttachment>) { ret |= eRender; }
		return ret;
	}

	void attach(std::unique_ptr<Attachment>&& attachment, TypeId type, std::uint8_t sub_type);
	Ptr<Attachment> find(TypeId type) const;
	bool detach(TypeId type);

	struct Impl;
	std::unique_ptr<Impl> m_impl{};
};
} // namespace tg
