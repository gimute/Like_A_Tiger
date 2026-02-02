#include "stdafx.h"
#include "GameInScene.h"

#include "Actor\Player\Player.h"
#include "Actor\Player\PlayerController.h"
#include "Actor\Player\PlayerCameraController.h"
#include "Camera\CameraManager.h"

#include "Actor\Enemy\EnemyManager.h"
#include "UI\EnemysHpGauge.h"

#include "Battle\BattleManager.h"

#include "Actor\BackGround\ProtoStage.h"

#include "GameScene\UpdateOrder.h"

#include "UI/PoseMenu.h"
#include "UI/PouseMenuManager.h"

#include "Inventory/Inventory.h"
#include "Inventory/Item.h"
#include "SaveManager.h"

#include "Actor\YakuzaComponents\YakuzaAttackAssistSystem.h"

#include "Battle\BattleAreaManager.h"
#include "Load\LoadManager.h"

#include "GameScene\GameResultScene.h"
#include "GameScene\GameOverScene.h"
#include "GameScene/GameTitleScene.h"

#include "UI/MiniMap.h"

//ステート侵入関数
void GameInScene::EnterScene()
{
	//戦闘開始でBGMを変えるコールバック登録
	BattleManager::GetInstance()->RegisterBattleStartCallBack(
		[&](const BattleStartEventInfo& eventInfo)
		{
			BgmManager::GetInstance()->RequestPlayBgm(bgm_inGame_battle, 0.5f);
		}
	);

	BattleManager::GetInstance()->RegisterBattleEndCallBack(
		[&](const BattleEndEventInfo& eventInfo)
		{
			BgmManager::GetInstance()->RequestStopBgm();

			BgmManager::GetInstance()->RequestPlayBgm(bgm_inGame_Explore, 0.5f);
		}
	);

	//プレイヤーコントローラー生成
	m_playerController = NewGO<PlayerController>(UpdateOrder::Input, "playercontroller");
	//プレイヤー生成
	m_player = NewGO<Player>(UpdateOrder::Charactar, "player");
	//カメラ生成
	CameraManager::GetCameraManagerInstance()->CreateCamera<PlayerCameraController>();
	//プレイヤーコントローラー設定
	m_playerController->SetPlayer(m_player);
	m_playerController->SetPlayerCameraController(CameraManager::GetCameraManagerInstance()->GetCameraController<PlayerCameraController>());
	//カメラ生成

	//攻撃アシストを初期化
	YakuzaAttackAssistSystem::GetIstance()->InitAttackAssistSystem(m_player);

	//敵マネージャー初期化
	EnemyManager::GetInstance()->InitEnemyManager();

	//敵HPを生成
	m_enemysHpGauge = NewGO<EnemysHpGauge>(UpdateOrder::UI, "enemy");

	//敵生成テスト
	EnemyManager::GetInstance()->RequestSpawnEnemyGroup(4,Vector3{ -1000.0f,0.0f,0.0f },true);
	EnemyManager::GetInstance()->RequestSpawnEnemyGroup(4,Vector3{ -3000.0f,0.0f,0.0f },true);
	EnemyManager::GetInstance()->RequestSpawnEnemyGroup(4,Vector3{ 1000.0f,0.0f,0.0f },true);
	//エネミーのターゲットを設定
	EnemyManager::GetInstance()->SetEnemyTargetCharacter(m_player);

	//ミニマップ用意
	m_miniMap = NewGO<MiniMap>(0);
	m_miniMap->SetPlayer(m_player);
	m_miniMap->SetPosition(Vector3(-600.0f, -300.0f, 0.0f));

	//戦闘マネージャー初期化
	BattleManager::GetInstance()->InitBattleManager();
	//プロトステージ生成
	m_protoStage = NewGO<ProtoStage>(UpdateOrder::Actor);

	m_inventory = Inventory::Create();

	m_poseMenu = NewGO<PoseMenu>(0, "posemenu");
	m_poseMenu->Init();


	/** マネージャーの起動セットアップ */
	auto* manager = PouseMenuSceneManager::GetSceneManagerInstance();
	manager->InitPouseMenuSceneManager();
	/** 操作対象のUIをマネージャーに登録 */
	manager->SetMenuOwner(m_poseMenu);
	/** 最初のステートを開く演出に設定 */
	//manager->RequestInitSceneState<PouseMenuOutSideScene>();


	/** デバッグテスト */
	//m_inSelect = NewGO<InSelect>(0);

	//セーブマネージャー生成
	SaveManager::GetInstance().Load();
	
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

   m_gameState = GameState::en_gameLoad;

   m_skyCube = NewGO<SkyCube>(0, "skycube");

   m_skyCube->SetLuminance(0.9f);

   BgmManager::GetInstance()->RequestPlayBgm(bgm_inGame_Explore, 0.5f);
}

//ステート更新関数
void GameInScene::UpdateScene()
{
	GameStateUpdate();

	//カメラ更新
	CameraManager::GetCameraManagerInstance()->UpdateCamera();

	//戦闘エリア更新
	BattleAreaManager::GetInstance()->Update(m_player->GetPosition());

	//EnemyManager更新
	EnemyManager::GetInstance()->Update();

	//戦闘マネージャー更新
	BattleManager::GetInstance()->Update();

	/** ポーズメニューマネージャー更新 */
	PouseMenuSceneManager::GetSceneManagerInstance()->UpdatePouseMenuSceneManager();

	/** タイトルへの遷移リクエストを確認 */
	if (PouseMenuSceneManager::GetSceneManagerInstance()->IsRequestReturnToTitle()) {
		m_gameState = GameState::en_returnToTitle;
		LoadManager::GetInstance()->LoadStart(3.0f);
	}

	if(!m_poseMenu){
	if (m_inventory)
	{
		m_inventory->ForEach([&](int index, ItemInfo* itemInfo) {
			ItemIconInformation* info = new ItemIconInformation();
			info->m_type = itemInfo->m_type;
			m_poseMenu->AddItemInfo(info);
			});
	}
	m_poseMenu->Init();
	}

	//バトルエリアの数とミニマップに表示されているバトルエリアの数に変化があれば
	if (EnemyManager::GetInstance()->GetCurrentEnemyGroupeNum() != m_miniMap->GetBattleAreaNum())
	{
		m_miniMap->ButtleAreaDataUpdate();
	}
}

void GameInScene::GameStateUpdate()
{
	switch (m_gameState)
	{
	case en_gameLoad:

		if (!LoadManager::GetInstance()->LoadFadeInEnd())
		{
			LoadManager::GetInstance()->LoadEnd();

			m_gameState = GameState::en_gameUpdate;
		}

		break;
	case en_gameUpdate:

		//敵残りグループ数による処理テスト
		if (0 >= EnemyManager::GetInstance()->GetCurrentEnemyGroupeNum())
		{
			m_gameState = GameState::en_gameClear;
		}

		if (0 >= m_player->GetYakuzaCurrentHp())
		{
			m_gameState = GameState::en_gameOver;
		}

		break;
	case en_gameOver:

		//プレイヤーのアニメーションが終わったら
		if (m_player->GetYakuzaStateMachine().IsHasCharactarPlayAnimation())
		{
			LoadManager::GetInstance()->LoadStart(3.0f);
		}

		break;
	case en_gameClear:

		LoadManager::GetInstance()->LoadStart(3.0f);

		break;
	}
}

//ステート退出関数
void GameInScene::ExitScene()
{
	//オブジェクト削除処理
	DeleteGameObjects();
}

void GameInScene::DeleteGameObjects()
{
	//プレイヤーコントローラー
	DeleteGO(m_playerController);
	//攻撃アシスト解除
	YakuzaAttackAssistSystem::GetIstance()->RemoveAttackAssistSystem();
	//敵全体を削除
	EnemyManager::GetInstance()->RequestResetEnemysProcees();
	//戦闘エリアをリセット
	BattleAreaManager::GetInstance()->ResetAreaManager();
	//バトルマネージャーをリセット
	BattleManager::GetInstance()->ResetBattleManager();
	//ステージ削除
	DeleteGO(m_protoStage);
	//エネミーのHP削除
	DeleteGO(m_enemysHpGauge);
	//プレイヤーの削除
	DeleteGO(m_player);
	//カメラ削除
	CameraManager::GetCameraManagerInstance()->DeleteCamera();
	//インベントリ
	Inventory::Delete();
	//マップ削除
	DeleteGO(m_miniMap);
	//スカイキューブ
	DeleteGO(m_skyCube);
	//ポーズメニュー削除
	PouseMenuSceneManager::GetSceneManagerInstance()->RequestInitSceneState<PouseMenuOutSideScene>();
	if (m_poseMenu) {
		DeleteGO(m_poseMenu);
		m_poseMenu = nullptr;
	}
}

//ステート変更要求関数
bool GameInScene::ReqestSceneState(uint32_t& nextState)
{
	if (m_gameState == GameState::en_gameClear &&
		LoadManager::GetInstance()->LoadFadeOutEnd())
	{
		nextState = GameResultScene::ID();

		return true;
	}
	else if (m_gameState == GameState::en_gameOver &&
		LoadManager::GetInstance()->LoadFadeOutEnd())
	{
		nextState = GameOverScene::ID();

		return true;
	}
	else if (m_gameState == GameState::en_returnToTitle &&
		LoadManager::GetInstance()->LoadFadeOutEnd())
	{
		nextState = GameTitleScene::ID();
		return true;
	}

	return false;
}