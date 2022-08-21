#pragma once
#include <tardigrade/scene.hpp>
#include <tardigrade/services.hpp>

namespace tg {
class Director {
  public:
	using Service = ServiceProvider<Director>;

	Director() = default;
	Director& operator=(Director&&) = delete;

	template <std::derived_from<Scene> T, typename... Args>
	T& enqueue(Args&&... args) {
		auto t = std::make_unique<T>(std::forward<Args>(args)...);
		auto ret = t.get();
		load(std::move(t));
		return *ret;
	}

	void load(std::unique_ptr<Scene>&& scene);
	bool active() const { return m_active != nullptr; }
	bool queued() const { return m_queued != nullptr; }

	void tick(Time dt);
	void render(RenderTarget const& target) const;

	float time_scale() const { return m_time_scale; }
	Director& set_time_scale(float scale);

  private:
	std::unique_ptr<Scene> m_queued{};
	std::unique_ptr<Scene> m_active{};
	float m_time_scale{1.0f};
};
} // namespace tg
