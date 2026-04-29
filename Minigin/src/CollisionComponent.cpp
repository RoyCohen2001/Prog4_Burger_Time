#include "CollisionComponent.h"
#include "GameObject.h"

#include <algorithm>

namespace dae
{
	std::vector<CollisionComponent*> CollisionComponent::s_AllColliders{};

	CollisionComponent::CollisionComponent(
		GameObject* owner,
		const glm::vec2& size,
		const glm::vec2& offset,
		CollisionLayer layer,
		bool isTrigger)
		: Component(owner)
		, m_size(size)
		, m_offset(offset)
		, m_isTrigger(isTrigger)
		, m_layer(layer)
	{
		s_AllColliders.push_back(this);
	}

	CollisionComponent::~CollisionComponent()
	{
		s_AllColliders.erase(
			std::remove(s_AllColliders.begin(), s_AllColliders.end(), this),
			s_AllColliders.end());
	}

	AABB CollisionComponent::GetAABB() const
	{
		const auto pos = GetOwner()->GetWorldPosition();
		return AABB{
			pos.x + m_offset.x,
			pos.y + m_offset.y,
			pos.x + m_offset.x + m_size.x,
			pos.y + m_offset.y + m_size.y
		};
	}

	float CollisionComponent::GetCenterX() const
	{
		const auto box = GetAABB();
		return (box.left + box.right) * 0.5f;
	}

	float CollisionComponent::GetCenterY() const
	{
		const auto box = GetAABB();
		return (box.top + box.bottom) * 0.5f;
	}

	float CollisionComponent::GetFeetY() const
	{
		return GetAABB().bottom;
	}

	bool CollisionComponent::IsOverlapping(const CollisionComponent& other) const
	{
		if (&other == this) return false;
		return GetAABB().Intersects(other.GetAABB());
	}

	std::vector<CollisionComponent*> CollisionComponent::GetOverlaps(bool includeTriggers) const
	{
		std::vector<CollisionComponent*> overlaps{};
		const auto myBox = GetAABB();

		for (auto* other : s_AllColliders)
		{
			if (!other || other == this) continue;
			if (!includeTriggers && (IsTrigger() || other->IsTrigger())) continue;

			if (myBox.Intersects(other->GetAABB()))
				overlaps.push_back(other);
		}
		return overlaps;
	}

	std::vector<CollisionComponent*> CollisionComponent::GetOverlapsByLayer(CollisionLayer layer, bool includeTriggers) const
	{
		std::vector<CollisionComponent*> result{};
		for (auto* other : GetOverlaps(includeTriggers))
		{
			if (other->GetLayer() == layer)
				result.push_back(other);
		}
		return result;
	}

}