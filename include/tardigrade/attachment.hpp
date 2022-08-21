#pragma once
#include <tardigrade/entity.hpp>

namespace tg {
class Attachment {
  public:
	virtual ~Attachment() = default;

	virtual void setup() = 0;

	Ptr<Scene> scene() const { return m_scene; }
	Ptr<Entity> entity() const;

  private:
	Ptr<Scene> m_scene{};
	Entity::Id m_entity{};

	friend class Entity;
};
} // namespace tg
