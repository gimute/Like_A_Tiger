#include "stdafx.h"
#include "GameInScene.h"

#include "Actor\Player\Player.h"
#include "Actor\Player\PlayerController.h"
#include "Actor\Player\PlayerCameraController.h"
#include "Camera\CameraManager.h"

#include "Actor\Enemy\EnemyManager.h"
#include "Actor\Enemy\EnemySystem.h"

#include "Actor\BackGround\ProtoStage.h"

#include "GameScene\UpdateOrder.h"

#include "UI\PoseMenu.h"

//ステート侵入関数
void GameInScene::EnterScene()
{
	//プレイヤーコントローラー生成
	m_playerController = NewGO<PlayerController>(UpdateOrder::Input, "playercontroller");
	//プレイヤー生成
	m_player = NewGO<Player>(UpdateOrder::Charactar, "player");
	//カメラ生成
	CameraManager::GetCameraManagerInstance()->CreateCamera<PlayerCameraController>(m_playerController);
	//プレイヤーコントローラー設定
	m_playerController->SetPlayer(m_player);
	m_playerController->SetPlayerCameraController(CameraManager::GetCameraManagerInstance()->GetCameraController<PlayerCameraController>());
	//カメラ生成

	//敵AI生成
	NewGO<EnemySystem>(UpdateOrder::AI, "enemy");

	//敵生成テスト
	EnemyManager::GetInstance()->RequestSpawnEnemy(EnemyType::en_normalYakuza,Vector3{1000.0,0.0,0.0});

	EnemyManager::GetInstance()->SetEnemyTargetCharacter(m_player);

	NewGO<ProtoStage>(UpdateOrder::Actor);

	m_poseMenu = NewGO<PoseMenu>(0, "posemenu");
	m_poseMenu->Init();
}

//ステート更新関数
void GameInScene::UpdateScene()
{
	//カメラ更新
	CameraManager::GetCameraManagerInstance()->UpdateCamera();

	//EnemyManager更新
	EnemyManager::GetInstance()->Update();
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