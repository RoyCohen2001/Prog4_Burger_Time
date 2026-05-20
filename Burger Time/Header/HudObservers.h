#pragma once
#include <string>
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class TextComponent;
	class GameActor;

	class LivesObserverComponent final : public Component, public Observer
	{
	public:
		LivesObserverComponent(GameObject* owner, TextComponent* text);

		void OnNotify(const std::string& event, GameObject* gameObject) override;

	private:
		TextComponent* m_text{};
	};

	class ScoreObserverComponent final : public Component, public Observer
	{
	public:
		ScoreObserverComponent(GameObject* owner, TextComponent* text);

		void OnNotify(const std::string& event, GameObject* gameObject) override;

	private:
		TextComponent* m_text{};
	};

	class PepperObserverComponent final : public Component, public Observer
	{
		public :
		PepperObserverComponent(GameObject* owner, TextComponent* text);

		void OnNotify(const std::string& event, GameObject* gameObject) override;

	private:
		TextComponent* m_text{};
	};
}