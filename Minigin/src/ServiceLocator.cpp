#include "ServiceLocator.h"
#include "NullSoundService.h"

namespace
{
	dae::NullSoundService g_NullSoundService{};
}

dae::SoundService* dae::ServiceLocator::m_pSoundService = &g_NullSoundService;

void dae::ServiceLocator::Provide(SoundService* pSoundService)
{
	m_pSoundService = pSoundService ? pSoundService : &g_NullSoundService;
}

dae::SoundService& dae::ServiceLocator::GetSoundService()
{
	return *m_pSoundService;
}