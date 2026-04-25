#pragma once
#include <filesystem>

namespace dae
{
	std::filesystem::path ResolveDataPath();
	void LoadGame();
}