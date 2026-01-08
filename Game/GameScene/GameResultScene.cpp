#include "stdafx.h"
#include "GameResultScene.h"

#include "GameScene\UpdateOrder.h"
#include "Load\LoadManager.h"
#include "GameScene\GameTitleScene.h"

void GameResultScene::EnterScene()
{
	m_resultSprite = NewGO<Result>(UpdateOrder::UI,"resultSprite");

	m_resultSprite->Init();
}

void GameResultScene::UpdateScene()
{
	switch (m_resultState)
	{
		//待機中
		case en_Standby:
		{
			//もし処理に入ってきた時にロード画面が暗転しきっていたら
			if (LoadManager::GetInstance()->LoadFadeOutEnd())
			{
				//ロードを明けさせる
				LoadManager::GetInstance()->LoadEnd();
				//ステートをロード明け待機
				m_resultState = ResultState::en_LoadIn;
			}
			//もしロードにそもそも入って無ければ
			else if (LoadManager::GetInstance()->LoadFadeInEnd())
			{
				//そのままボタン押し待機
				m_resultState = ResultState::en_WaitButtonTrigger;
			}

			break;
		}
		//ロード明け待機
		case en_LoadIn:
		{
			//ロードが明けたら
			if (LoadManager::GetInstance()->LoadFadeInEnd())
			{
				//ボタン押し待機
				m_resultState = ResultState::en_WaitButtonTrigger;
			}

			break;
		}
		//ボタンが押されるまで待機
		case en_WaitButtonTrigger:
		{
			//ボタンが押されたら
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//ロード暗転待機に
				m_resultState = ResultState::en_ProceesEnd;
			}

			break;
		}
		//リザルト処理終了
		case en_ProceesEnd:
		{
			//ロード暗転開始
			LoadManager::GetInstance()->LoadStart(3.0f);
			break;
		}
			
	}
}

void GameResultScene::ExitScene()
{

}

bool GameResultScene::ReqestSceneState(uint32_t& nextState)
{
	if (m_resultState == ResultState::en_ProceesEnd &&
		LoadManager::GetInstance()->LoadFadeOutEnd())
	{
		nextState = GameTitleScene::ID();

		return true;
	}

	return false;
}