#include "stdafx.h"
#include "GameInScene.h"

#include "Actor\Player\Player.h"

//ステート侵入関数
void GameInScene::EnterScene()
{

	NewGO<Player>(0, "player");

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