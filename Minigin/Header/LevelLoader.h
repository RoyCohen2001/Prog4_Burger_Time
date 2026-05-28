#pragma once
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace dae
{
    class Scene;

    class LevelLoader final
    {
    public:
        static void LoadLevelFromJson(const std::string& filename, Scene& scene, const glm::vec2& levelOffset = glm::vec2{ 0,0 });

        static float GetGroundYAt(float worldX, float currentY);
        static bool IsOnLadder(float worldX, float worldY);

        static glm::vec4 GetLevelBounds();
        static float GetHighestGroundYAt(float worldX);
        static float GetLowestGroundYAt(float worldX);
    private:
        static std::string GetTextureForType(const std::string& type);
    };
}