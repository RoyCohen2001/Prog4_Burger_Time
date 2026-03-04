#include <stdexcept>
#include <iostream>
#include <algorithm>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "ThrashTheCache.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	m_renderer = SDL_CreateRenderer(window, nullptr);
	if (m_renderer == nullptr)
	{
		std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
#if __EMSCRIPTEN__
	io.IniFilename = NULL;
#endif

	ImGui_ImplSDL3_InitForSDLRenderer(window, m_renderer);
	ImGui_ImplSDLRenderer3_Init(m_renderer);
}

void dae::Renderer::Render() const
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	// --- Trash the Cache benchmark window ---
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
	// ----------------------------------------

	ImGui::Render();

	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	dst.w = width;
	dst.h = height;
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }