#include "stdafx.h"
#include "Game.h"

//ゲームシーンマネージャー関連
#include "GameScene\GameSceneManager.h"
#include "GameScene\GameTitleScene.h"
#include "GameScene\GameInScene.h"
#include "GameScene\GameResultScene.h"
#include "GameScene\GameOverScene.h"

#include "InvisibleWall.h"

#include "Load\LoadManager.h"

#include "Sound\SoundId.h"

bool Game::Start()
{
	//ゲーム中で使うエフェクトの準備
	InitEffect();

	//ゲーム中で使うエフェクトの準備
	InitSound();

	//初期化処理は必要であれば追加予定
	GameSceneManager::GetSceneManagerInstance()->InitGameSceneManager();

	//ゲームタイトルのシーンを追加
	GameSceneManager::GetSceneManagerInstance()->AddSceneState<GameTitleScene>();
	//ゲームインシーンを追加
	GameSceneManager::GetSceneManagerInstance()->AddSceneState<GameInScene>(); 
	//ゲームのリザルトシーンを追加
	GameSceneManager::GetSceneManagerInstance()->AddSceneState<GameResultScene>();
	//ゲームオーバーのシーンを追加
	GameSceneManager::GetSceneManagerInstance()->AddSceneState<GameOverScene>();

	//初期化はゲームタイトルシーンにする
	GameSceneManager::GetSceneManagerInstance()->ReqestInitSceneState<GameTitleScene>();
	GameSceneManager::GetSceneManagerInstance()->CurrentStateEnter();

	//ロードマネージャーを初期化
	LoadManager::GetInstance()->InitLoadManager();

	//m_anim[0].Load("Assets/modelData/Character/Survivalist/Animation/Idle.tka");
	//m_anim[0].SetLoopFlag(true);
	//m_anim[1].Load("Assets/modelData/Character/Survivalist/Animation/CrossPunch_R_Ev.tka");
	//m_anim[1].SetLoopFlag(false);
	//
	//m_testmodel.Init("Assets/modelData/Character/Survivalist/Survivalist.tkm", m_anim, 2);

	//m_effectBone = m_testmodel.GetBone(L"middle_01_r");

	//Vector3 m_effectPos;

	//m_effectBone->GetWorldMatrix().Apply(m_effectPos);

	return true;
}

void Game::Update()
{

	//if (g_pad[0]->IsTrigger(enButtonA))
	//{
	//	if (testeffect != nullptr)
	//	{
	//		testeffect->Stop();
	//	}

	//	m_testmodel.PlayAnimation(1);
	//	auto handle = EffectManager::Get().PlayEffect(5, Vector3::Zero, Vector3::One * 10.0f);
	//	testeffect = EffectManager::Get().FindEffect(handle);
	//}

	//if (testeffect != nullptr)
	//{
	//	Vector3 m_effectPos = Vector3::Zero;

	//	m_effectBone->GetWorldMatrix().Apply(m_effectPos);

	//	testeffect->SetPosition(m_effectPos);
	//}
	//
	//if (!m_testmodel.IsPlayingAnimation())
	//{
	//	m_testmodel.PlayAnimation(0);
	//}

	//m_testmodel.Update();

	//ゲームシーンマネージャーの更新
	GameSceneManager::GetSceneManagerInstance()->UpdateGameSceneState();
	//ロードマネージャーの更新
	LoadManager::GetInstance()->Update();

}

void Game::Render(RenderContext& rc)
{
	//m_testmodel.Draw(rc);
}

void Game::InitEffect()
{
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/Effect/InvisibleWall/Circle.efk");
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/Effect/InvisibleWall/Ring.efk");
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/Effect/InvisibleWall/Wall.efk");
	EffectEngine::GetInstance()->ResistEffect(4, u"Assets/Effect/InvisibleWall/WallLine.efk");
	EffectEngine::GetInstance()->ResistEffect(5, u"Assets/Effect/efk/test/test_1.efk");
}

void Game::InitSound()
{
	g_soundEngine->ResistWaveFileBank(se_hittingLightA, "Assets/Sound/HittingSound/hitting_light_A.wav");	//パンチヒット音
	g_soundEngine->ResistWaveFileBank(se_hittingLightB, "Assets/Sound/HittingSound/hitting_light_B.wav");	//パンチヒット音
	g_soundEngine->ResistWaveFileBank(se_hittingHeavyA, "Assets/Sound/HittingSound/hitting_heavy_A.wav");	//パンチヒット音
	g_soundEngine->ResistWaveFileBank(se_hittingHeavyB, "Assets/Sound/HittingSound/hitting_heavy_B.wav");	//パンチヒット音
	g_soundEngine->ResistWaveFileBank(se_hittingDefenseA, "Assets/Sound/HittingSound/hitting_defense_A.wav");	//パンチヒット音
	g_soundEngine->ResistWaveFileBank(se_cuttingWindLigthA, "Assets/Sound/CuttingWindSound/cuttingWind_light_A.wav");	//風切り音
	g_soundEngine->ResistWaveFileBank(se_cuttingWindHeavyA, "Assets/Sound/CuttingWindSound/cuttingWind_heavy_A.wav");	//風切り音
	g_soundEngine->ResistWaveFileBank(se_FootstepsA, "Assets/Sound/YakuzaMoveingSound/Footsteps_A.wav");	//足音
	g_soundEngine->ResistWaveFileBank(se_kickingGroundA, "Assets/Sound/YakuzaMoveingSound/KickingGround_A.wav");	//地面を蹴る音
	g_soundEngine->ResistWaveFileBank(se_GroundFrictionA, "Assets/Sound/YakuzaMoveingSound/GroundFriction_A.wav");	//地面をこする音
}
