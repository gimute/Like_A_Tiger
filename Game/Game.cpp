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

	return true;
}

void Game::Update()
{

	//ゲームシーンマネージャーの更新
	GameSceneManager::GetSceneManagerInstance()->UpdateGameSceneState();
	//ロードマネージャーの更新
	LoadManager::GetInstance()->Update();

}

void Game::Render(RenderContext& rc)
{

}

void Game::InitEffect()
{
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/Effect/InvisibleWall/Circle.efk");
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/Effect/InvisibleWall/Ring.efk");
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/Effect/InvisibleWall/Wall.efk");
	EffectEngine::GetInstance()->ResistEffect(4, u"Assets/Effect/InvisibleWall/WallLine.efk");
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
}
