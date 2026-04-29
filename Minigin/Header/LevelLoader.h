#pragma once
#include <string>
#include <glm/vec2.hpp>

namespace dae
{
    class Scene;

    class LevelLoader final
    {
    public:
        static void LoadLevelFromJson(const std::string& filename, Scene& scene, const glm::vec2& levelOffset = glm::vec2{ 0,0 });

        static float GetGroundYAt(float worldX, float currentY);
        static bool IsOnLadder(float worldX, float worldY);
    private:
        static std::string GetTextureForType(const std::string& type);
    };
}