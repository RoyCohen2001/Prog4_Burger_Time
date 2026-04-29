#include "LevelLoader.h"
#include <array>
#include <fstream>
#include <limits>
#include <vector>
#include <glm/vec4.hpp>
#include <nlohmann/json.hpp>
#include "GameObject.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "CollisionComponent.h"

#ifndef __EMSCRIPTEN__
#include <Windows.h>
#include <sstream>
#include <filesystem>
#endif

using json = nlohmann::json;

namespace
{
    constexpr float kIngredientYOffset = 45.f;
    constexpr float kIngredientScale = 2.f;
    constexpr float kLadderProbeMargin = 12.f;

    // x, y, w, h
    std::vector<glm::vec4> s_groundRects{};
    std::vector<glm::vec4> s_ladderRects{};

    bool IsIngredientType(const std::string& type)
    {
        return type == "patty"
            || type == "cheese"
            || type == "lettuce"
            || type == "bottombun"
            || type == "bun"
            || type == "tomato";
    }

    bool IsGroundType(const std::string& type)
    {
        return type == "platform"
            || type == "ladderplatform"
            || type == "plate";
    }

    bool IsLadderType(const std::string& type)
    {
        return type == "ladder";
    }

    std::string GetIngredientFrameTexturePath(const std::string& type, int frameIndex)
    {
        return "Sprites/Ingredients/" + type + std::to_string(frameIndex) + ".png";
    }
}

float dae::LevelLoader::GetGroundYAt(float worldX, float currentY)
{
    float bestY = std::numeric_limits<float>::infinity();

    for (const auto& rect : s_groundRects)
    {
        const float left = rect.x;
        const float top = rect.y;
        const float right = rect.x + rect.z;

        const bool xInside = (worldX >= left && worldX < right);
        const bool atOrBelow = (top >= currentY);

        if (xInside && atOrBelow && top < bestY)
        {
            bestY = top;
        }
    }

    return bestY;
}

bool dae::LevelLoader::IsOnLadder(float worldX, float worldY)
{
    for (const auto& rect : s_ladderRects)
    {
        const float left = rect.x - kLadderProbeMargin;
        const float right = rect.x + rect.z + kLadderProbeMargin;
        const float top = rect.y;
        const float bottom = rect.y + rect.w;

        const bool xInside = (worldX >= left && worldX < right);
        const bool yInside = (worldY >= top && worldY < bottom);

        if (xInside && yInside)
        {
            return true;
        }
    }

    return false;
}

std::string dae::LevelLoader::GetTextureForType(const std::string& type)
{
    if (type == "ladder") return "Sprites/World/ladder.png";
    if (type == "platform") return "Sprites/World/platform.png";
    if (type == "ladderplatform") return "Sprites/World/ladderplatform.png";
    if (type == "plate") return "Sprites/World/plate.png";

    if (IsIngredientType(type)) return "";

    return "";
}

void dae::LevelLoader::LoadLevelFromJson(const std::string& filename, Scene& scene, const glm::vec2& levelOffset)
{
    s_groundRects.clear();
    s_ladderRects.clear();

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open JSON file: " + filename);
    }

    json levelData;
    file >> levelData;

    for (const auto& object : levelData)
    {
        const std::string type = object["type"];
        const bool isIngredient = IsIngredientType(type);

        std::shared_ptr<dae::Texture2D> singleTexture{};
        std::array<std::shared_ptr<dae::Texture2D>, 4> ingredientTextures{};

        if (isIngredient)
        {
            for (int i = 0; i < 4; ++i)
            {
                ingredientTextures[i] = dae::ResourceManager::GetInstance().LoadTexture(GetIngredientFrameTexturePath(type, i));
            }
        }
        else
        {
            const std::string texturePath = GetTextureForType(type);
            if (!texturePath.empty())
            {
                singleTexture = dae::ResourceManager::GetInstance().LoadTexture(texturePath);
            }

#ifndef __EMSCRIPTEN__
            std::ostringstream oss;
            oss << "Loading from this path: " << texturePath << " with type: " << type << "\n";
            OutputDebugStringA(oss.str().c_str());
#endif
        }

        for (const auto& pos : object["positions"])
        {
            const float baseX = pos[0] + levelOffset.x;
            const float baseY = pos[1] + levelOffset.y;

            if (isIngredient)
            {
                float currentX = baseX;
                const float y = baseY + kIngredientYOffset;

                for (int i = 0; i < 4; ++i)
                {
                    const auto& frameTexture = ingredientTextures[i];
                    if (!frameTexture) continue;

                    auto go = std::make_unique<dae::GameObject>();
                    go->SetPosition(currentX, y);

                    auto* render = go->AddComponent<dae::RenderComponent>();
                    render->SetTexture(frameTexture);
                    render->SetScale(kIngredientScale);

                    go->AddComponent<dae::CollisionComponent>(
                        render->GetScaledSize(),
                        glm::vec2{ 0.f, 0.f },
                        dae::CollisionLayer::Trigger,
                        true);

                    scene.Add(std::move(go));
                    currentX += frameTexture->GetSize().x * kIngredientScale;
                }
            }
            else
            {
                auto go = std::make_unique<dae::GameObject>();
                go->SetPosition(baseX, baseY);

                if (singleTexture)
                {
                    auto* render = go->AddComponent<dae::RenderComponent>();
                    render->SetTexture(singleTexture);

                    auto* collision = go->AddComponent<dae::CollisionComponent>(render->GetScaledSize());

                    if (IsGroundType(type))
                    {
						float sizeY = 6.f;
                        s_groundRects.emplace_back(baseX, baseY, render->GetScaledSize().x, sizeY);
						collision->SetLayer(dae::CollisionLayer::Platform);
						collision->SetSize({ render->GetScaledSize().x, sizeY });
						collision->SetOffset({ 0.f, render->GetScaledSize().y - sizeY });
                    }
                    if (IsLadderType(type))
                    {
						float sizeX = 50.f;
                        s_ladderRects.emplace_back(baseX, baseY, sizeX, render->GetScaledSize().y);
                        collision->SetLayer(dae::CollisionLayer::Ladder);
                        collision->SetSize({ sizeX, render->GetScaledSize().y });
						collision->SetOffset({ (render->GetScaledSize().x - sizeX) / 2.f, 0.f });
						collision->SetTrigger(true);
                    }
                }

                scene.Add(std::move(go));
            }
        }
    }
}