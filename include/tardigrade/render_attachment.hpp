#pragma once
#include <tardigrade/render_target.hpp>
#include <tardigrade/tick_attachment.hpp>

namespace tg {
class RenderAttachment : public TickAttachment {
  public:
	virtual void render(RenderTarget const& target) const = 0;

	int layer() const { return m_layer; }

  protected:
	int m_layer{};
};
} // namespace tg
