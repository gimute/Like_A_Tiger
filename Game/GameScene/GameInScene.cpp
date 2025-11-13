#include "stdafx.h"
#include "GameInScene.h"

#include "Actor\Player\Player.h"
#include "Actor\Player\PlayerController.h"
#include "Actor\Player\PlayerCameraController.h"
#include "Camera\CameraManager.h"
#include "UI\HPGauge.h"

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
	//HPゲージ生成
	m_HPGauge = NewGO<HPGauge>(0, "HPGauge");
	m_HPGauge->SetPosition(Vector3(-600.0f, 320.0f, 0.0f));

	HPGauge::HPGaugeUIInitData defaultHPGauge =
	{
		"Assets/spriteData/HPGauge/Test/HPGauge_Frame.DDS",
		1024,
		32,
		"Assets/spriteData/HPGauge/Test/HPGauge_BarA_Texture.DDS",
		"Assets/spriteData/HPGauge/Test/HPGauge_BarB_Texture.DDS",
		1004,
		22
	};

	m_HPGauge->Init();
}

//ステート更新関数
void GameInScene::UpdateScene()
{
	if(g_pad[0]->IsTrigger(enButtonA))
	{
		m_HPGauge->SetHP(m_nowHP -= 0.1);
	}

	if (g_pad[0]->IsTrigger(enButtonB))
	{
		m_HPGauge->SetHP(m_nowHP += 0.1);
	}

	if (g_pad[0]->IsTrigger(enButtonX))
	{
		m_nowHP -= 0.1f;
		m_HPGauge->DecreaseHP(0.1f);
	}

	if (g_pad[0]->IsTrigger(enButtonY))
	{
		m_nowHP += 0.1f;
		m_HPGauge->IncreaseHP(0.1f);
	}



	//m_HPGauge->SetHP(m_nowHP -= 0.1 * g_gameTime->GetFrameDeltaTime());
	

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