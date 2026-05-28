#include "IngredientComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "LevelLoader.h"

#include <limits>

namespace
{
	constexpr float kPieceDropOffset = 6.f;
	constexpr float kPlateTolerance = 0.5f;
}

std::vector<dae::IngredientComponent*> dae::IngredientComponent::s_ingredients{};

dae::IngredientComponent::IngredientComponent(GameObject* owner)
	: Component(owner)
{
	s_ingredients.push_back(this);
}

dae::IngredientComponent::~IngredientComponent()
{
	auto it = std::remove(s_ingredients.begin(), s_ingredients.end(), this);
	s_ingredients.erase(it, s_ingredients.end());
}

size_t dae::IngredientComponent::RegisterPiece(GameObject* piece)
{
	PieceInfo info{};
	info.object = piece;
	info.collider = piece ? piece->GetComponent<CollisionComponent>() : nullptr;
	info.baseLocalPos = piece ? piece->GetTransform().GetLocalPosition() : glm::vec3{ 0.f };
	info.hit = false;

	m_pieces.push_back(info);
	return m_pieces.size() - 1;
}

void dae::IngredientComponent::OnPieceHit(size_t index)
{
	if (m_isFalling || m_onPlate || index >= m_pieces.size())
		return;

	auto& piece = m_pieces[index];
	if (piece.hit || !piece.object)
		return;

	piece.hit = true;
	piece.object->SetLocalPosition(piece.baseLocalPos + glm::vec3{ 0.f, kPieceDropOffset, 0.f });

	if (AllPiecesHit())
	{
		StartFall();
	}
}

void dae::IngredientComponent::Update()
{
	if (!m_isFalling)
		return;

	const float dt = TimeManager::GetInstance().GetDeltaTime();
	auto localPos = GetOwner()->GetTransform().GetLocalPosition();
	const float nextY = localPos.y + m_fallSpeed * dt;

	if (nextY >= m_targetTopY - kPlateTolerance)
	{
		localPos.y = m_targetTopY;
		m_isFalling = false;
		m_onPlate = m_targetIsPlate;
		ResetPieces();
	}
	else
	{
		localPos.y = nextY;
	}

	GetOwner()->SetLocalPosition(localPos);
}

void dae::IngredientComponent::StartFall()
{
	if (m_isFalling || m_onPlate)
		return;

	const auto bounds = GetBounds();
	if (bounds.right <= bounds.left || bounds.bottom <= bounds.top)
		return;

	const float height = bounds.bottom - bounds.top;
	const float centerX = (bounds.left + bounds.right) * 0.5f;
	const float groundY = LevelLoader::GetGroundYAt(centerX, bounds.bottom + 1.f);

	if (!std::isfinite(groundY) || groundY <= bounds.bottom + kPlateTolerance)
		return;

	m_targetTopY = groundY - height;
	m_targetIsPlate = LevelLoader::IsPlateTopAt(centerX, groundY);

	m_isFalling = true;
	TriggerBelowFall();
}

void dae::IngredientComponent::ResetPieces()
{
	for (auto& piece : m_pieces)
	{
		piece.hit = false;
		if (piece.object)
		{
			piece.object->SetLocalPosition(piece.baseLocalPos);
			if (auto* pieceComp = piece.object->GetComponent<IngredientPieceComponent>())
			{
				pieceComp->ResetHit();
			}
		}
	}
}

bool dae::IngredientComponent::AllPiecesHit() const
{
	for (const auto& piece : m_pieces)
	{
		if (!piece.hit)
			return false;
	}
	return !m_pieces.empty();
}

dae::AABB dae::IngredientComponent::GetBounds() const
{
	AABB bounds{};
	bounds.left = std::numeric_limits<float>::infinity();
	bounds.top = std::numeric_limits<float>::infinity();
	bounds.right = -std::numeric_limits<float>::infinity();
	bounds.bottom = -std::numeric_limits<float>::infinity();

	for (const auto& piece : m_pieces)
	{
		if (!piece.collider)
			continue;

		const auto aabb = piece.collider->GetAABB();
		bounds.left = std::min(bounds.left, aabb.left);
		bounds.top = std::min(bounds.top, aabb.top);
		bounds.right = std::max(bounds.right, aabb.right);
		bounds.bottom = std::max(bounds.bottom, aabb.bottom);
	}

	if (!std::isfinite(bounds.left) || !std::isfinite(bounds.top))
	{
		bounds = {};
	}

	return bounds;
}

void dae::IngredientComponent::TriggerBelowFall()
{
	const auto myBounds = GetBounds();
	if (myBounds.right <= myBounds.left)
		return;

	IngredientComponent* bestBelow{};
	float bestTop = std::numeric_limits<float>::infinity();

	for (auto* other : s_ingredients)
	{
		if (!other || other == this || other->IsFalling() || other->IsOnPlate())
			continue;

		const auto otherBounds = other->GetBounds();
		const bool xOverlap = (myBounds.right > otherBounds.left && myBounds.left < otherBounds.right);

		if (xOverlap && otherBounds.top > myBounds.top && otherBounds.top < bestTop)
		{
			bestTop = otherBounds.top;
			bestBelow = other;
		}
	}

	if (bestBelow)
	{
		bestBelow->StartFall();
	}
}

dae::IngredientPieceComponent::IngredientPieceComponent(GameObject* owner, IngredientComponent* parent, size_t index)
	: Component(owner), m_parent(parent), m_index(index)
{}

void dae::IngredientPieceComponent::Update()
{
	if (m_hit || !m_parent)
		return;

	if (!m_collision)
	{
		m_collision = GetOwner()->GetComponent<CollisionComponent>();
		if (!m_collision)
			return;
	}

	const auto overlaps = m_collision->GetOverlapsByLayer(CollisionLayer::Player, true);
	if (!overlaps.empty())
	{
		m_hit = true;
		m_parent->OnPieceHit(m_index);
	}
}