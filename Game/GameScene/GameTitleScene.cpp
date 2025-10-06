#include "stdafx.h"
#include "GameTitleScene.h"

#include "GameScene\GameInScene.h"

//ステート侵入関数
void GameTitleScene::EnterScene()
{

	int test = 0;

	test += 1;

}

//ステート更新関数
void GameTitleScene::UpdateScene()
{

	int testU = 0;

	testU += 1;

}

//ステート退出関数
void GameTitleScene::ExitScene()
{
	int testE = 0;

	testE += 1;
}

//ステート変更要求関数
bool GameTitleScene::ReqestSceneState(uint32_t& nextState)
{
	nextState = GameInScene::ID();

	return true;
}

