#pragma once

#include "Component.h"

namespace dae
{
    class ImGuiComponent : public Component
    {
    public:
        ImGuiComponent(GameObject* owner);
        void RenderImGui(); 
    };
}