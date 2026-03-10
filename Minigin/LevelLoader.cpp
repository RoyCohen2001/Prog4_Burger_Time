#include "LevelLoader.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "GameObject.h"
#include "RenderComponent.h"
#include "Scene.h"

#ifndef __EMSCRIPTEN__
#include <Windows.h>
#include <sstream>
#include <filesystem>
#endif

using json = nlohmann::json;

std::string dae::LevelLoader::GetTextureForType(const std::string& type)
{
    // Map type to texture file
    if (type == "ladder") return "Sprites/World/ladder.png";
    if (type == "platform") return "Sprites/World/platform.png";
    if (type == "ladderplatform") return "Sprites/World/ladderplatform.png";
    if (type == "plate") return "Sprites/World/plate.png";

    return "";
}

void dae::LevelLoader::LoadLevelFromJson(const std::string& filename, Scene& scene, const glm::vec2& levelOffset)
{
    std::ifstream file(filename); 
    if (!file.is_open()) {
        throw std::runtime_error("Could not open JSON file: " + filename);
    }
    json levelData;
    file >> levelData;

    for (const auto& object : levelData)
    {
        const std::string type = object["type"];
        std::string texture = GetTextureForType(type);

#ifndef __EMSCRIPTEN__
        std::ostringstream oss;
		oss << "Loading from this path: " << texture << " with type: " << type << "\n";
        OutputDebugStringA(oss.str().c_str());
#endif
       
       
       for (const auto& pos : object["positions"])
       {
           float x = pos[0] + levelOffset.x;
           float y = pos[1] + levelOffset.y;
       
           auto go = std::make_unique<dae::GameObject>();
           go->SetPosition(x, y);
       
           if (!texture.empty())
               go->AddComponent<dae::RenderComponent>(texture);
       
       
           scene.Add(std::move(go));
       }
    }
}


