#include "stdafx.h"
#include "LoadManager.h"

#include "GameScene\UpdateOrder.h"

//これを有効にするとデバッグモードになる
//#define DEBUG_MODE

LoadManager* LoadManager::m_instance = nullptr;

void LoadManager::InitLoadManager()
{
	m_loadScreenSprite = NewGO<LoadScreenSprite>(UpdateOrder::LoadUI, "loadScreen");
}

void LoadManager::LoadStart(float fadeInWaitTime)
{
	if (m_loadManagerProcees != LoadManagerProcees::en_loadStandby)
	{
		return;
	}

	m_fadeInWaitTime = fadeInWaitTime;
	//フェードアウト開始
	m_loadScreenSprite->FadeOutExecuting(1.0f);
	//フェードイン実行フラグリセット
	m_fadeInExecutingFlag = false;

	m_loadManagerProcees = LoadManagerProcees::en_loadFadeOutExecuting;
}

void LoadManager::LoadEnd()
{
	if (m_loadManagerProcees != LoadManagerProcees::en_loadFadeOutWait)
	{
		return;
	}

	m_fadeInExecutingFlag = true;
}

void LoadManager::Update()
{
	//テスト用
	#ifdef DEBUG_MODE

	if (g_pad[0]->IsTrigger(enButtonB))
	{
		LoadStart(1.0f);
	}
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		LoadEnd();
	}

	if (m_loadScreenSprite->IsFadeOut())
	{
		int test01 = 0;
	}
	if (m_loadScreenSprite->IsFadeIn())
	{
		int test02 = 0;
	}

	#endif

	switch (m_loadManagerProcees)
	{
		case en_loadFadeOutExecuting:
		{
			//ロード画面スプライトが暗転しきったら
			if (m_loadScreenSprite->IsFadeOut())
			{
				m_loadManagerProcees = LoadManagerProcees::en_loadFadeOutWait;
			}

			break;
		}
		case en_loadFadeOutWait:
		{

			//ロードが明られるなら
			if (m_fadeInExecutingFlag)
			{
				//フェード明け待機処理
				m_fadeInWaitTime -= g_gameTime->GetFrameDeltaTime();

				if (m_fadeInWaitTime <= 0.0f)
				{
					m_fadeInWaitTime = 0.0f;

					//フェードイン処理に入る
					m_loadScreenSprite->FadeInExecuting();

					m_loadManagerProcees = LoadManagerProcees::en_loadFadeInExecuting;
				}
			}

			break;
		}
		case en_loadFadeInExecuting:
		{
			//ロードが明けきったら
			if (m_loadScreenSprite->IsFadeIn())
			{
				m_loadManagerProcees = LoadManagerProcees::en_loadCompletion;
			}

			break;
		}
		case en_loadCompletion:
		{
			//待機状態に戻す
			m_loadManagerProcees = LoadManagerProcees::en_loadStandby();

			break;
		}
	}
}