#include "stdafx.h"
#include "GameOverScene.h"

#include "GameScene\UpdateOrder.h"
#include "GameScene\GameOver\GameOver.h"
#include "GameScene\GameTitleScene.h"
#include "Load\LoadManager.h"

void GameOverScene::EnterScene()
{
	m_gameOverSprite = NewGO<GameOver>(UpdateOrder::UI, "sprite");

	m_gameOverSprite->Init();

	m_gameOverState = GameOverState::en_GameOverStandby;
}

void GameOverScene::UpdateScene()
{
	switch (m_gameOverState)
	{
		case en_GameOverStandby:
		{
			//もし処理に入ってきた時にロード画面が暗転しきっていたら
			if (LoadManager::GetInstance()->LoadFadeOutEnd())
			{
				//ロードを明けさせる
				LoadManager::GetInstance()->LoadEnd();
				//ステートをロード明け待機
				m_gameOverState = GameOverState::en_GameOverLoadIn;
			}
			//もしロードにそもそも入って無ければ
			else if (LoadManager::GetInstance()->LoadFadeInEnd())
			{
				//そのままボタン押し待機
				m_gameOverState = GameOverState::en_GameOverWaitButton;
			}

			break;
		}
		case en_GameOverLoadIn:
		{
			//ロードが明けたら
			if (LoadManager::GetInstance()->LoadFadeInEnd())
			{
				//ボタン押し待機
				m_gameOverState = GameOverState::en_GameOverWaitButton;
			}

			break;
		}
		case en_GameOverWaitButton:
		{
			//ボタンが押されたら
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//ロード暗転待機に
				m_gameOverState = GameOverState::en_GameOverProceesEnd;
			}

			break;
		}
		case en_GameOverProceesEnd:
		{
			//ロード暗転開始
			LoadManager::GetInstance()->LoadStart(3.0f);

			break;
		}
	}
}

void GameOverScene::ExitScene()
{
	DeleteGO(m_gameOverSprite);
}

bool GameOverScene::ReqestSceneState(uint32_t& nextState)
{
	if (m_gameOverState == GameOverState::en_GameOverProceesEnd &&
		LoadManager::GetInstance()->LoadFadeOutEnd())
	{
		nextState = GameTitleScene::ID();

		return true;
	}

	return false;
}