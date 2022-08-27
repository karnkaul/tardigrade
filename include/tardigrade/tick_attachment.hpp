#pragma once
#include <tardigrade/attachment.hpp>
#include <tardigrade/detail/delta_time.hpp>

namespace tg {
class TickAttachment : public Attachment {
  public:
	virtual void tick(DeltaTime dt) = 0;

	int order{};
};
} // namespace tg
