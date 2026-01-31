#include "stdafx.h"
#include "BgmManager.h"

#include "Sound\SoundManager.h"

namespace BgmManagerConstant
{
	float BGM_LEAP_MAX_RATE = 1.0f;
	float BGM_LEAP_MIN_RATE = 0.0f;
}

void BgmManager::RequestPlayBgm(int kindBgmNo)
{
	SoundManager::Get().PlayBGM(kindBgmNo);
}

void BgmManager::RequestStopBgm()
{
	SoundManager::Get().StopBGM();
}

void BgmManager::RequestChangeBgm(BgmFadeFormat format)
{
	//フェードアウトフラグセット
	m_bgmFadeFlag[BgmFadeState::en_fadeout] = format.m_bgmFadeOutFlag;
	//フェードインフラグセット
	m_bgmFadeFlag[BgmFadeState::en_fadein] = format.m_bgmFadeInFlag;

	//フェードアウトスピードセット
	m_bgmFadeSpeed[BgmFadeState::en_fadeout] = format.m_bgmFadeOutSpeed;
	//フェードインスピードセット
	m_bgmFadeFlag[BgmFadeState::en_fadein] = format.m_bgmFadeInSpeed;

	//次に再生するBGM番号セット
	m_nextBgmNo = format.m_preNextBgmNo;

	//フェードアウトステートへ移行
	m_bgmChangeState = BgmFadeState::en_fadeout;
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

		//もしフェードアウトをしない設定なら即座にBGM停止してフェードインステートへ
		if (!m_bgmFadeFlag[BgmFadeState::en_fadeout])
		{
			SoundManager::Get().StopBGM();

			m_bgmChangeState = BgmFadeState::en_fadein;
		}
		else
		{
			m_bgmFadeLerpRate += g_gameTime->GetFrameDeltaTime() * m_bgmFadeSpeed[BgmFadeState::en_fadein];

			lerpRate = Leap(
				BgmManagerConstant::BGM_LEAP_MAX_RATE,
				BgmManagerConstant::BGM_LEAP_MIN_RATE,
				m_bgmFadeLerpRate
			);

			SoundManager::Get().SetBGMVolume(lerpRate);

			if (lerpRate <= 0.0f)
			{
				SoundManager::Get().StopBGM();

				m_bgmChangeState = BgmFadeState::en_fadein;
			}
		}

		break;
	case en_fadeout:

		//もしフェードインをしない設定なら即座にBGM再生してレディステートへ
		if (!m_bgmFadeFlag[BgmFadeState::en_fadein])
		{
			SoundManager::Get().PlayBGM(m_nextBgmNo);

			m_bgmChangeState = BgmFadeState::en_ready;
		}
		else
		{
			m_bgmFadeLerpRate += g_gameTime->GetFrameDeltaTime() * m_bgmFadeSpeed[BgmFadeState::en_fadeout];

			lerpRate = Leap(
				BgmManagerConstant::BGM_LEAP_MIN_RATE,
				BgmManagerConstant::BGM_LEAP_MAX_RATE,
				m_bgmFadeLerpRate
			);

			SoundManager::Get().SetBGMVolume(lerpRate);

			if (lerpRate >= 1.0f)
			{
				SoundManager::Get().PlayBGM(m_nextBgmNo);

				m_bgmChangeState = BgmFadeState::en_ready;

				//数値類を初期化
				m_nextBgmNo = -1;
				m_bgmFadeLerpRate = 0.0f;
			}
		}

		break;
	case en_ready:
		break;
	default:
		break;
	}
}