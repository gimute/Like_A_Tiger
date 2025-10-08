#include "stdafx.h"
#include "GameInScene.h"

//ステート侵入関数
void GameInScene::EnterScene()
{

	m_player = NewGO<TestPlayer>(0, "testplayer");

	m_enemy = NewGO<TestEnemy>(0, "testenemy");

}

//ステート更新関数
void GameInScene::UpdateScene()
{

}

//ステート退出関数
void GameInScene::ExitScene()
{

}

//ステート変更要求関数
bool GameInScene::ReqestSceneState(uint32_t& nextState)
{
	return false;
}