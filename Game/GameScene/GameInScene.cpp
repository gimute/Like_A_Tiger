#include "stdafx.h"
#include "GameInScene.h"

#include "Actor\Player\Player.h"
#include "Actor\Player\PlayerController.h"
#include "Actor\Player\PlayerCameraController.h"
#include "Camera\CameraManager.h"

#include "Actor\BackGround\ProtoStage.h"

//ステート侵入関数
void GameInScene::EnterScene()
{
	//プレイヤー生成
	m_player = NewGO<Player>(0, "player");
	//プレイヤーコントローラー設定
	m_playerController = NewGO<PlayerController>(0, "playercontroller");
	m_playerController->TargetSet(m_player);
	//カメラ生成
	CameraManager::GetCameraManagerInstance()->CreateCamera<PlayerCameraController>(m_player);

	NewGO<ProtoStage>(0);
}

//ステート更新関数
void GameInScene::UpdateScene()
{
	//カメラ更新
	CameraManager::GetCameraManagerInstance()->UpdateCamera();
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