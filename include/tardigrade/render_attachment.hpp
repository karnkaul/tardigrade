#pragma once
#include <tardigrade/render_target.hpp>
#include <tardigrade/tick_attachment.hpp>

namespace tg {
class RenderAttachment : public TickAttachment {
  public:
	static constexpr auto tick_order_v = 100;

	void setup() override { order = tick_order_v; }
	virtual void render(RenderTarget const& target) const = 0;

	int layer{};
};
} // namespace tg
