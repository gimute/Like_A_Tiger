#include "stdafx.h"
#include "Game.h"

//ゲームシーンマネージャー関連
#include "GameScene\GameSceneManager.h"
#include "GameScene\GameTitleScene.h"
#include "GameScene\GameInScene.h"


bool Game::Start()
{

	m_image = m_canvas.CreateUI<UIImage>();
	m_image->Init("Assets/spriteData/HPGauge/Test/HPGauge_BarA_Texture.DDS",100.0f,100.0f);


	std::vector<Vector3> targetTranslateList = { Vector3(0.0f, 0.0f, 0.0f), Vector3(200.0f, 0.0f, 0.0f), Vector3(0.0f, 200.0f, 0.0f) };
	std::vector<float> timeList = { 1.0f,1.0f };
	m_posanim = std::make_unique<PositionUIAnimation>(m_image, true, timeList, targetTranslateList);
	m_posanim->Play();

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
	m_canvas.Update();

	m_posanim->Update();

	//ゲームシーンマネージャーの更新
	GameSceneManager::GetSceneManagerInstance()->UpdateGameSceneState();

}

void Game::Render(RenderContext& rc)
{
	m_canvas.Render(rc);
}
