#pragma once
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include "Transform.h"
#include "Component.h"

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	private:
		GameObject* m_parent{ nullptr };
		std::vector<GameObject*> m_children{};

		Transform m_transform{};
		std::vector<std::unique_ptr<Component>> m_components{};

		void SetPositionDirty();
		bool IsChild(const GameObject* potentialChild) const;
		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);

	public:
		void Update();
		void Render() const;

		void SetPosition(float x, float y);
		const Transform& GetTransform() const { return m_transform; }

		void SetParent(GameObject* parent, bool keepWorldPosition);
		GameObject* GetParent() const { return m_parent; }
		size_t GetChildCount() const { return m_children.size(); }
		GameObject* GetChildAt(unsigned int index) const { return m_children[index]; }

		void SetLocalPosition(const glm::vec3& pos);
		glm::vec3 GetWorldPosition();

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template <typename ComponentType, typename... Args>
		ComponentType* AddComponent(Args&&... args)
		{
			std::unique_ptr<ComponentType> component{ std::make_unique<ComponentType>(this, std::forward<Args>(args)...) };
			ComponentType* ptr{ component.get() };
			m_components.push_back(std::move(component));
			return ptr;
		}

		template <typename ComponentType>
		void RemoveComponent()
		{
			for (const auto& component : m_components)
			{
				if (ComponentType* castedComponent = dynamic_cast<ComponentType*>(component.get()))
				{
					castedComponent->SetDelete();
				}
			}
		}

		void ProcessComponentRemoval()
		{
			auto it = std::remove_if(m_components.begin(), m_components.end(),
				[](const std::unique_ptr<Component>& component)
				{
					return component->ShouldBeDeleted();
				});

			m_components.erase(it, m_components.end());
		}

		template <typename ComponentType>
		ComponentType* GetComponent() const
		{
			for (const auto& component : m_components)
			{
				if (ComponentType* castedComponent = dynamic_cast<ComponentType*>(component.get()))
				{
					return castedComponent;
				}
			}

			return nullptr;
		}

		template <typename ComponentType>
		bool HasComponent() const
		{
			return GetComponent<ComponentType>() != nullptr;
		}
	};
}