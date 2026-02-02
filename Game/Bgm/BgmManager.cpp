#include "stdafx.h"
#include "BgmManager.h"

#include "Sound\SoundManager.h"

namespace BgmManagerConstant
{
	float BGM_LEAP_MAX_RATE = 1.0f;
	float BGM_LEAP_MIN_RATE = 0.0f;
}

//インスタンス初期化
BgmManager* BgmManager::m_instance = nullptr;

void BgmManager::RequestPlayBgm(int kindBgmNo,float volume,bool isFadeIn)
{
	if (isFadeIn)
	{
		SoundManager::Get().PlayBGM(kindBgmNo);
		SoundManager::Get().SetBGMVolume(0.0f);

		m_bgmChangeState = BgmFadeState::en_fadein;

		return;
	}

	SoundManager::Get().PlayBGM(kindBgmNo);
	SoundManager::Get().SetBGMVolume(volume);
}

void BgmManager::RequestStopBgm(bool isFadeOut)
{
	if (isFadeOut)
	{
		m_bgmChangeState = BgmFadeState::en_fadeout;

		return;
	}

	SoundManager::Get().StopBGM();
}

void BgmManager::Update()
{
	//もしBGM切り替えステートがレディステートでなければフェードアウト更新処理を行う
	if (m_bgmChangeState != BgmFadeState::en_ready)
	{
		BgmFadeOutUpdate();
	}
}

void BgmManager::BgmFadeOutUpdate()
{
	float lerpRate = 0.0f;
	
	switch (m_bgmChangeState)
	{
	case en_fadein:

		m_bgmFadeLerpRate += g_gameTime->GetFrameDeltaTime() * m_bgmFadeSpeed[BgmFadeState::en_fadein];

		lerpRate = Leap(
			BgmManagerConstant::BGM_LEAP_MAX_RATE,
			BgmManagerConstant::BGM_LEAP_MIN_RATE,
			m_bgmFadeLerpRate
		);

		SoundManager::Get().SetBGMVolume(lerpRate);

		if (lerpRate <= 0.0f)
		{
			m_bgmChangeState = BgmFadeState::en_ready;

			//数値類を初期化
			m_nextBgmNo = -1;
			m_bgmFadeLerpRate = 0.0f;
		}

		break;
	case en_fadeout:

		m_bgmFadeLerpRate += g_gameTime->GetFrameDeltaTime() * m_bgmFadeSpeed[BgmFadeState::en_fadeout];

		lerpRate = Leap(
			BgmManagerConstant::BGM_LEAP_MIN_RATE,
			BgmManagerConstant::BGM_LEAP_MAX_RATE,
			m_bgmFadeLerpRate
		);

		SoundManager::Get().SetBGMVolume(lerpRate);

		if (lerpRate >= 1.0f)
		{
			m_bgmChangeState = BgmFadeState::en_ready;

			//数値類を初期化
			m_nextBgmNo = -1;
			m_bgmFadeLerpRate = 0.0f;
		}

		break;
	case en_ready:
		break;
	default:
		break;
	}
}