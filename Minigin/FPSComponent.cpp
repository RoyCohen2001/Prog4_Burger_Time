#include "FPSComponent.h"
#include "GameObject.h"
#include "TextObject.h"
#include "TimeManager.h"

using namespace dae;

FPSComponent::FPSComponent(GameObject* owner)
    : Component(owner)
{
    //m_textComponent = owner->GetComponent<TextComponent>();
}

void FPSComponent::Update()
{
    auto& timeManager = TimeManager::GetInstance();
    const float deltaTime = timeManager.GetDeltaTime();

    
}