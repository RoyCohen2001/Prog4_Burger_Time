#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include "TimeManager.h"

namespace dae
{
	class Minigin final
	{
		TimeManager& m_time = TimeManager::GetInstance();

		bool m_quit{};
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}