#include "stdafx.h"
#include "GameInScene.h"

#include "Actor\Player\Player.h"
#include "Actor\Player\PlayerController.h"
#include "Actor\Player\PlayerCameraController.h"
#include "Camera\CameraManager.h"

#include "Actor\Enemy\EnemyManager.h"
#include "Actor\Enemy\EnemySystem.h"
#include "UI\EnemysHpGauge.h"

#include "Battle\BattleManager.h"

#include "Actor\BackGround\ProtoStage.h"

#include "GameScene\UpdateOrder.h"

#include "UI\PoseMenu.h"

#include "Inventory/Inventory.h"

#include "Actor\YakuzaComponents\YakuzaAttackAssistSystem.h"

#include "Battle\BattleAreaManager.h"
#include "Load\LoadManager.h"


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

	//攻撃アシストを初期化
	YakuzaAttackAssistSystem::GetIstance()->InitAttackAssistSystem(m_player);
	
	//敵AI生成
	NewGO<EnemySystem>(UpdateOrder::AI, "enemy");

	//敵HPを生成
	NewGO<EnemysHpGauge>(UpdateOrder::UI, "enemy");

	//敵生成テスト
	//EnemyManager::GetInstance()->RequestSpawnEnemy(EnemyYakuzaType::en_normalYakuza,Vector3{1000.0,0.0,0.0});
	//EnemyManager::GetInstance()->RequestSpawnEnemyGroup(4,Vector3{ 1000.0f,0.0f,0.0f });
	EnemyManager::GetInstance()->RequestSpawnEnemyGroup(4,Vector3{ -1000.0f,0.0f,0.0f });
	EnemyManager::GetInstance()->RequestSpawnEnemyGroup(4,Vector3{ 1000.0f,0.0f,0.0f });
	EnemyManager::GetInstance()->RequestSpawnEnemyGroup(4,Vector3{ -3000.0f,0.0f,0.0f });

	EnemyManager::GetInstance()->SetEnemyTargetCharacter(m_player);

	NewGO<ProtoStage>(UpdateOrder::Actor);

	m_inventory = Inventory::Create();

	m_poseMenu = NewGO<PoseMenu>(0, "posemenu");


   // UIへ情報を渡す
   {
   	m_inventory->ForEach([&](const ItemInfo* itemInfo)
   		{
   			ItemIconInformation* info = new ItemIconInformation();
   			info->m_num = itemInfo->m_num;
   			info->m_type = itemInfo->m_type;
   			m_poseMenu->AddItemInfo(info);
   		});
   }
	//m_poseMenu->Init();

	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

//ステート更新関数
void GameInScene::UpdateScene()
{
	if (!LoadManager::GetInstance()->LoadFadeInEnd())
	{
		LoadManager::GetInstance()->LoadEnd();
	}

	//カメラ更新
	CameraManager::GetCameraManagerInstance()->UpdateCamera();

	//戦闘エリア更新
	BattleAreaManager::GetInstance()->Update(m_player->GetPosition());

	//EnemyManager更新
	EnemyManager::GetInstance()->Update();

	//戦闘マネージャー更新
	BattleManager::GetInstance()->Update();

	///   // UIへ情報を渡す
	///   {
	///   	m_inventory->ForEach([&](const ItemInfo* itemInfo)
	///   		{
	///   			ItemIconInformation* info = new ItemIconInformation();
	///   			info->m_num = itemInfo->m_num;
	///   			info->m_type = itemInfo->m_type;
	///   			m_poseMenu->AddItemInfo(info);
	///   		});
	///   }

	//敵残りグループ数による処理テスト
	if (0 >= EnemyManager::GetInstance()->GetCurrentEnemyGroupeNum())
	{
		bool test = true;
	}
}

//ステート退出関数
void GameInScene::ExitScene()
{
	Inventory::Delete();
}

//ステート変更要求関数
bool GameInScene::ReqestSceneState(uint32_t& nextState)
{
	return false;
}