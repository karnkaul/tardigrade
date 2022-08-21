#include <tardigrade/attachment.hpp>
#include <tardigrade/scene.hpp>

namespace tg {
Ptr<Entity> Attachment::entity() const { return m_scene ? m_scene->find(m_entity) : nullptr; }
} // namespace tg
