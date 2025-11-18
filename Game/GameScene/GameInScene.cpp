#include "stdafx.h"
#include "GameInScene.h"

#include "Actor\Player\Player.h"
#include "Actor\Player\PlayerController.h"
#include "Actor\Player\PlayerCameraController.h"
#include "Camera\CameraManager.h"

#include "Actor\Enemy\EnemyManager.h"

#include "Actor\BackGround\ProtoStage.h"

//ステート侵入関数
void GameInScene::EnterScene()
{
	//プレイヤーコントローラー生成
	m_playerController = NewGO<PlayerController>(0, "playercontroller");
	//プレイヤー生成
	m_player = NewGO<Player>(0, "player");
	//カメラ生成
	CameraManager::GetCameraManagerInstance()->CreateCamera<PlayerCameraController>(m_playerController);
	//プレイヤーコントローラー設定
	m_playerController->SetPlayer(m_player);
	m_playerController->SetPlayerCameraController(CameraManager::GetCameraManagerInstance()->GetCameraController<PlayerCameraController>());
	//カメラ生成

	//敵生成テスト
	EnemyManager::GetInstance()->RequestSpawnEnemy(EnemyType::en_normalYakuza,Vector3::Zero);

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