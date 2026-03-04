#include "stdafx.h"
#include "GameTitleScene.h"

#include "GameScene\GameInScene.h"
#include "Load\LoadManager.h"

//ステート侵入関数
void GameTitleScene::EnterScene()
{
	m_titleSprite = NewGO<Title>(0, "titleSprite");
	m_titleSprite->Deactivate();

	m_titleState = TitleState::en_TitleStandby;
}

//ステート更新関数
void GameTitleScene::UpdateScene()
{
	switch (m_titleState)
	{
		case en_TitleStandby:
		{
			//もし処理に入ってきた時にロード画面が暗転しきっていたら
			if (LoadManager::GetInstance()->LoadFadeOutEnd())
			{
				//ロードを明けさせる
				LoadManager::GetInstance()->LoadEnd();
				//ステートをロード明け待機
				m_titleState = TitleState::en_TitleLoadIn;
			}
			//もしロードにそもそも入って無ければ
			else if (LoadManager::GetInstance()->LoadFadeInEnd())
			{
				m_titleSprite->Activate();

				//そのままボタン押し待機
				m_titleState = TitleState::en_TitleWaitButtonTrigger;
			}
			break;
		}
		case en_TitleLoadIn:
		{
			//ロードが明けたら
			if (LoadManager::GetInstance()->LoadFadeInEnd())
			{
				//ボタン押し待機
				m_titleState = TitleState::en_TitleWaitButtonTrigger;
				
				m_titleSprite->Activate();
			}

			break;
		}
		case en_TitleWaitButtonTrigger:
		{
			//ボタンが押されたら
			if (g_pad[0]->IsTrigger(enButtonB))
			{
				//ロード暗転待機に
				m_titleState = TitleState::en_TitleProceesEnd;
			}

			break;
		}
		case en_TitleProceesEnd:
		{
			//ロード暗転開始
			LoadManager::GetInstance()->LoadStart(3.0f);

			break;
		}
	}
}

//ステート退出関数
void GameTitleScene::ExitScene()
{
	DeleteGO(m_titleSprite);
}

//ステート変更要求関数
bool GameTitleScene::ReqestSceneState(uint32_t& nextState)
{
	if (m_titleState == TitleState::en_TitleProceesEnd &&
		LoadManager::GetInstance()->LoadFadeOutEnd())
	{
		nextState = GameInScene::ID();

		return true;
	}

	return false;
}

