#include <tardigrade/detail/delta_time.hpp>
#include <tardigrade/director.hpp>
#include <cassert>

namespace tg {
void Director::load(std::unique_ptr<Scene>&& scene) {
	if (scene) {
		m_queued = std::move(scene);
		m_queued->setup();
	}
}

void Director::tick(Time dt) {
	if (m_queued) { m_active = std::move(m_queued); }
	if (!m_active) { return; }
	m_active->tick({dt, m_time_scale});
}

void Director::render(RenderTarget const& target) const {
	if (!m_active) { return; }
	m_active->render(target);
}

Director& Director::set_time_scale(float scale) {
	if (scale >= 0.0f) { m_time_scale = scale; }
	return *this;
}
} // namespace tg
