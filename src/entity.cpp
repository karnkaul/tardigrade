#include <tardigrade/detail/render_list.hpp>
#include <tardigrade/detail/type_id_hasher.hpp>
#include <tardigrade/entity.hpp>
#include <tardigrade/render_attachment.hpp>
#include <tardigrade/tick_attachment.hpp>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

namespace tg {
struct Entity::Impl {
	enum Flag : std::uint8_t { eNone = 0, eActive = 1 << 0, eDestroyed = 1 << 1 };

	std::unordered_map<TypeId, std::unique_ptr<Attachment>, TypeId::Hasher> attachments{};
	std::unordered_set<void*> tickables{};
	std::unordered_set<void const*> renderables{};
	std::vector<TickAttachment*> tick_list{};
	std::vector<RenderAttachment const*> render_list{};
	std::uint8_t flags{Flag::eActive};

	Ptr<Scene> scene{};
	Id id{};
};

Entity::Entity() : m_impl(std::make_unique<Impl>()) {}
Entity::Entity(Entity&&) noexcept = default;
Entity& Entity::operator=(Entity&&) noexcept = default;
Entity::~Entity() noexcept = default;

Entity::Entity(Ptr<Scene> scene, Id id) : Entity() {
	m_impl->scene = scene;
	m_impl->id = id;
}

auto Entity::id() const -> Id { return m_impl ? m_impl->id : Id::eDefault; }

Entity& Entity::set_active(bool active) {
	if (active) {
		m_impl->flags |= Impl::eActive;
	} else {
		m_impl->flags &= ~Impl::eActive;
	}
	return *this;
}

Entity& Entity::destroy() {
	m_impl->flags |= Impl::eDestroyed;
	return *this;
}

bool Entity::active() const { return (m_impl->flags & Impl::eActive) == Impl::eActive; }
bool Entity::destroyed() const { return (m_impl->flags & Impl::eDestroyed) == Impl::eDestroyed; }

void Entity::attach(std::unique_ptr<Attachment>&& attachment, TypeId type, std::uint8_t sub_type) {
	attachment->m_scene = m_impl->scene;
	attachment->m_entity = m_impl->id;
	attachment->setup();
	if ((sub_type & eRender) == eRender) { m_impl->renderables.insert(attachment.get()); }
	if ((sub_type & eTick) == eTick) { m_impl->tickables.insert(attachment.get()); }
	m_impl->attachments.insert_or_assign(type, std::move(attachment));
}

Ptr<Attachment> Entity::find(TypeId type) const {
	if (auto it = m_impl->attachments.find(type); it != m_impl->attachments.end()) { return it->second.get(); }
	return {};
}

bool Entity::detach(TypeId type) {
	if (auto it = m_impl->attachments.find(type); it != m_impl->attachments.end()) {
		m_impl->renderables.erase(it->second.get());
		m_impl->tickables.erase(it->second.get());
		m_impl->attachments.erase(it);
		return true;
	}
	return false;
}

std::size_t Entity::attached_count() const { return m_impl->attachments.size(); }

void Entity::clear_attached() {
	m_impl->attachments.clear();
	m_impl->tickables.clear();
	m_impl->renderables.clear();
}

void Entity::tick(DeltaTime dt) {
	m_impl->tick_list.clear();
	m_impl->render_list.clear();
	for (auto* tickable : m_impl->tickables) { m_impl->tick_list.push_back(static_cast<TickAttachment*>(tickable)); }
	static constexpr auto comp = [](TickAttachment* a, TickAttachment* b) { return a->order < b->order; };
	if (sort_ticks) { std::sort(m_impl->tick_list.begin(), m_impl->tick_list.end(), comp); }
	for (auto* tickable : m_impl->tick_list) {
		tickable->tick(dt);
		if (m_impl->renderables.contains(tickable)) { m_impl->render_list.push_back(static_cast<RenderAttachment const*>(tickable)); }
	}
}

void Entity::fill_render_list(RenderList& out) const { std::copy(m_impl->render_list.begin(), m_impl->render_list.end(), std::back_inserter(out.attachments)); }

Ptr<Scene> Entity::scene() const { return m_impl->scene; }
} // namespace tg
