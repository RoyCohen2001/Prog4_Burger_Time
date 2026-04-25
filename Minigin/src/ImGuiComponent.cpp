#include "ImGuiComponent.h"
#include "ThrashTheCache.h"
#include <imgui.h>

namespace dae
{
	ImGuiComponent::ImGuiComponent(GameObject* owner)
		: Component(owner)
	{
	}

    void ImGuiComponent::RenderImGui()
    {
		static std::vector<float> intResults{};
		static std::vector<float> goResults{};
		static std::vector<float> goAltResults{};

		ImGui::Begin("Thrash the Cache");

		if (ImGui::Button("Run Integer Benchmark"))
		{
			ttc::ThrashTheCache bench{};
			intResults = bench.RunIntegerBenchmark();
		}
		if (!intResults.empty())
		{
			const float maxVal = *std::max_element(intResults.begin(), intResults.end());
			ImGui::Text("Exercise 1: integers");
			ImGui::PlotLines("##int", intResults.data(), static_cast<int>(intResults.size()),
				0, nullptr, 0.f, maxVal, ImVec2(0, 80));
		}

		ImGui::Separator();

		if (ImGui::Button("Run GameObject Benchmark"))
		{
			ttc::ThrashTheCache bench{};
			goResults = bench.RunGameObjectBenchmark();
		}
		if (!goResults.empty())
		{
			const float maxVal = *std::max_element(goResults.begin(), goResults.end());
			ImGui::Text("Exercise 2: GameObjects");
			ImGui::PlotLines("##go", goResults.data(), static_cast<int>(goResults.size()),
				0, nullptr, 0.f, maxVal, ImVec2(0, 80));
		}

		if (!goAltResults.empty())
		{
			const float maxVal = *std::max_element(goAltResults.begin(), goAltResults.end());
			ImGui::Text("Exercise 2: GameObjects (Alt)");
			ImGui::PlotLines("##goAlt", goAltResults.data(), static_cast<int>(goAltResults.size()),
				0, nullptr, 0.f, maxVal, ImVec2(0, 80));
		}

		if (ImGui::Button("Run GameObject Alt Benchmark"))
		{
			ttc::ThrashTheCache bench{};
			goAltResults = bench.RunGameObjectAltBenchmark();
		}

		ImGui::End();
    }
}