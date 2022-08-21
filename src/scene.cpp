#include <tardigrade/detail/render_list.hpp>
#include <tardigrade/scene.hpp>
#include <algorithm>
#include <unordered_map>

namespace tg {
namespace {
Entity::Id& operator++(Entity::Id& in) { return in = Entity::Id{static_cast<std::underlying_type_t<Entity::Id>>(in) + 1}; }
} // namespace

struct Scene::Impl {
	std::unordered_map<Entity::Id, Entity> entities{};
	std::vector<Entity*> tick_list{};
	RenderList render_list{};

	Entity::Id next{Entity::Id::eDefault};
};

Scene::Scene() : m_impl(std::make_unique<Impl>()) {}
Scene::~Scene() noexcept = default;

Ptr<Entity> Scene::find(Entity::Id id) const {
	if (auto const it = m_impl->entities.find(id); it != m_impl->entities.end()) { return &it->second; }
	return {};
}

bool Scene::destroy(Entity::Id id) {
	if (auto e = find(id)) {
		e->destroy();
		return true;
	}
	return false;
}

std::size_t Scene::entity_count() const { return m_impl->entities.size(); }
void Scene::clear_entities() { m_impl->entities.clear(); }

void Scene::tick(DeltaTime dt) {
	m_impl->tick_list.clear();
	for (auto& [_, entity] : m_impl->entities) {
		if (entity.active()) { m_impl->tick_list.push_back(&entity); }
	}
	for (auto* entity : m_impl->tick_list) { entity->tick(dt); }
	for (auto it = m_impl->entities.begin(); it != m_impl->entities.end();) {
		if (it->second.destroyed()) {
			it = m_impl->entities.erase(it);
		} else {
			++it;
		}
	}
}

RenderList const& Scene::build_render_list() {
	m_impl->render_list.renderables.clear();
	for (auto const& [_, entity] : m_impl->entities) {
		if (entity.active()) { entity.fill_render_list(m_impl->render_list); }
	}
	if (sort_renders) {
		static constexpr auto comp = [](RenderAttachment const* a, RenderAttachment const* b) { return a->layer() < b->layer(); };
		std::sort(m_impl->render_list.renderables.begin(), m_impl->render_list.renderables.end(), comp);
	}
	return m_impl->render_list;
}

void Scene::render(RenderTarget const& target) {
	auto const list = build_render_list();
	for (auto const* renderable : list.renderables) { renderable->render(target); }
}

Ptr<Entity> Scene::spawn_impl() {
	auto const id = ++m_impl->next;
	auto entity = Entity{this, id};
	auto [it, _] = m_impl->entities.insert_or_assign(id, Entity{this, id});
	return &it->second;
}
} // namespace tg
