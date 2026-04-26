#pragma once
#include "SoundService.h"

namespace dae
{
	class ServiceLocator final
	{
	public:
		static void Provide(SoundService* pSoundService);
		static SoundService& GetSoundService();

	private:
		static SoundService* m_pSoundService;
	};
}