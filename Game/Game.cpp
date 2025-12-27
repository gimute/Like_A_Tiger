#include "stdafx.h"
#include "Game.h"

//ゲームシーンマネージャー関連
#include "GameScene\GameSceneManager.h"
#include "GameScene\GameTitleScene.h"
#include "GameScene\GameInScene.h"

#include "InvisibleWall.h"


bool Game::Start()
{
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/Effect/Circle.efk");
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/Effect/Ring.efk");
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/Effect/Wall.efk");
	EffectEngine::GetInstance()->ResistEffect(4, u"Assets/Effect/WallLine.efk");

	//初期化処理は必要であれば追加予定
	GameSceneManager::GetSceneManagerInstance()->InitGameSceneManager();

	//ゲームタイトルのシーンを追加
	GameSceneManager::GetSceneManagerInstance()->AddSceneState<GameTitleScene>();
	//ゲームインシーンを追加
	GameSceneManager::GetSceneManagerInstance()->AddSceneState<GameInScene>();

	//初期化はゲームタイトルシーンにする
	GameSceneManager::GetSceneManagerInstance()->ReqestInitSceneState<GameTitleScene>();

	return true;
}

void Game::Update()
{

	//ゲームシーンマネージャーの更新
	GameSceneManager::GetSceneManagerInstance()->UpdateGameSceneState();

}

void Game::Render(RenderContext& rc)
{

}
