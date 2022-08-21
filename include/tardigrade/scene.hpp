#pragma once
#include <tardigrade/entity.hpp>
#include <tardigrade/render_attachment.hpp>
#include <tardigrade/render_target.hpp>
#include <tardigrade/tick_attachment.hpp>

namespace tg {
class Scene {
  public:
	Scene();
	virtual ~Scene() noexcept;

	Scene& operator=(Scene&&) = delete;

	virtual void setup() {}

	template <std::derived_from<Attachment>... Types>
	Ptr<Entity> spawn() {
		auto ret = spawn_impl();
		if (ret) { (ret->attach<Types>(), ...); }
		return ret;
	}

	Ptr<Entity> find(Entity::Id id) const;
	bool contains(Entity::Id id) const { return find(id) != nullptr; }
	bool destroy(Entity::Id id);

	std::size_t entity_count() const;
	void clear_entities();

	void tick(DeltaTime dt);
	void render(RenderTarget const& target);

	RenderList const& build_render_list();

	bool sort_renders{true};

  private:
	Ptr<Entity> spawn_impl();

	struct Impl;
	std::unique_ptr<Impl> m_impl{};
};
} // namespace tg
