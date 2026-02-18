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
		Transform m_transform{};
		std::vector<std::unique_ptr<Component>> m_components{};

	public:
		virtual void Update();
		void Render() const;

		void SetPosition(float x, float y);
		const Transform& GetTransform() const { return m_transform; }

		GameObject() = default;
		virtual ~GameObject();
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