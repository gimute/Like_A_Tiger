#include "stdafx.h"
#include "GameSceneManager.h"

#include "IGameSceneState.h"

//インスタンスの初期化
GameSceneManager* GameSceneManager::m_SceneManagerInstance = nullptr;

void GameSceneManager::InitGameSceneManager()
{

}

void GameSceneManager::UpdateGameSceneState()
{
	if (m_currentState)
	{
		//ハッシュ値、リクエストされたステート
		uint32_t reqestState = 0;
		if (m_currentState->ReqestSceneState(reqestState))
		{
			//ステートの終了を実行
			m_currentState->ExitScene();
			//ステートをハッシュマップから探して変更
			m_currentState = FindState(reqestState);
			//ステートの初期化処理を実行
			m_currentState->EnterScene();
		}
	}
	//ステートを更新
	m_currentState->UpdateScene();

}