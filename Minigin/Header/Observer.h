#pragma once
#include <string>

namespace dae
{
	class GameObject;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(const std::string& event, GameObject* gameObject) = 0;

		Observer() = default;
		Observer(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		virtual Observer& operator=(const Observer& other) = delete;
		virtual Observer& operator=(Observer&& other) = delete;
	};
}