#include "stdafx.h"
#include "GameInScene.h"

//ステート侵入関数
void GameInScene::EnterScene()
{

	int testEn = 0;

	testEn += 1;

}

//ステート更新関数
void GameInScene::UpdateScene()
{

	int testUp = 0;

	testUp += 1;

}

//ステート退出関数
void GameInScene::ExitScene()
{

	int testEx = 0;

	testEx += 1;

}

//ステート変更要求関数
bool GameInScene::ReqestSceneState(uint32_t& nextState)
{
	return false;
}