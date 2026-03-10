#pragma once
#include <string>
#include <vector>
#include "GameObject.h"

namespace dae {
    class Scene;

    class LevelLoader final {
    public:
        static void LoadLevelFromJson(const std::string& filename, Scene& scene, const glm::vec2& levelOffset = glm::vec2{0,0});

    private:
        static std::string GetTextureForType(const std::string& type);
        static std::vector<std::shared_ptr<GameObject>> m_SolidBlocks;
    };
}