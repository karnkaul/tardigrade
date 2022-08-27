#pragma once
#include <tardigrade/detail/ptr.hpp>
#include <vector>

namespace tg {
class RenderAttachment;

struct RenderList {
	std::vector<Ptr<RenderAttachment const>> attachments{};
};
} // namespace tg
