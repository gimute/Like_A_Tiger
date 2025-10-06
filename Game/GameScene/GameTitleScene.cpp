#include "stdafx.h"
#include "GameTitleScene.h"

#include "GameScene\GameInScene.h"

//ステート侵入関数
void GameTitleScene::EnterScene()
{

}

//ステート更新関数
void GameTitleScene::UpdateScene()
{

}

//ステート退出関数
void GameTitleScene::ExitScene()
{

}

//ステート変更要求関数
bool GameTitleScene::ReqestSceneState(uint32_t& nextState)
{
	nextState = GameInScene::ID();

	return true;
}

