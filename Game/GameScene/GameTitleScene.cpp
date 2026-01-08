#include "stdafx.h"
#include "GameTitleScene.h"

#include "GameScene\GameInScene.h"
#include "Load\LoadManager.h"

//ステート侵入関数
void GameTitleScene::EnterScene()
{
	m_titleSprite = NewGO<Title>(0, "titleSprite");

	m_titleSprite->Init();
}

//ステート更新関数
void GameTitleScene::UpdateScene()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		LoadManager::GetInstance()->LoadStart(3.0f);
	}

	if (LoadManager::GetInstance()->LoadFadeOutEnd())
	{
		m_isGoNextScene = true;
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
	if (m_isGoNextScene)
	{
		nextState = GameInScene::ID();

		return true;
	}

	return false;
}

