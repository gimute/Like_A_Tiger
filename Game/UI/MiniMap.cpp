#include "stdafx.h"
#include "MiniMap.h"

#include "Actor/Player/Player.h"

MiniMap::MiniMap()
{
	ParameterManager::GetInstance().LoadParameter<MiniMapImageParameter>("Assets/Json/MiniMap.json", [](const nlohmann::json& j, MiniMapImageParameter& p)
		{
			p.mapImagePath = j["MapImagePath"].get<std::string>();
			p.mapImageWidth = j["MapImageWidth"].get<float>();
			p.mapImageHeight = j["MapImageHeight"].get<float>();
			p.playerIconImagePath = j["PlayerIconImagePath"].get<std::string>();
			p.enemyIconImagePath = j["EnemyIconImagePath"].get<std::string>();
			p.iconSize = j["IconSize"].get<float>();
		});

	auto* parameter = ParameterManager::GetInstance().GetParameter<MiniMapImageParameter>();


	m_canvas = std::make_shared<UICanvas>();

	m_mapImage = m_canvas->CreateUI<UIImage>();
	m_mapImage->Init(parameter->mapImagePath.c_str(), parameter->mapImageWidth, parameter->mapImageHeight);

	m_playerIcon = m_canvas->CreateUI<UIImage>();
	m_playerIcon->Init(parameter->playerIconImagePath.c_str(), parameter->iconSize, parameter->iconSize);

	m_enemyIconData.imageFilePath = parameter->enemyIconImagePath;
	m_enemyIconData.iconSize = parameter->iconSize;

	ParameterManager::GetInstance().UnloadParameter<MiniMapImageParameter>();
}

MiniMap::~MiniMap()
{

}

void MiniMap::Update()
{
	CalcBattlePointUIPos();

	m_canvas->Update();

}

void MiniMap::Render(RenderContext& rc)
{
	m_canvas->Render(rc);
}

void MiniMap::SetPlayer(Player* player)
{
	m_player = player;
}

void MiniMap::AddBattlePoint(Vector3 pos)
{
	BattlePointUIData battlePointUI;

	battlePointUI.battlePointIcon = m_canvas->CreateUI<UIImage>();
	battlePointUI.battlePointIcon->Init(m_enemyIconData.imageFilePath.c_str(), m_enemyIconData.iconSize, m_enemyIconData.iconSize);

	battlePointUI.battleAreaPos = pos;

	m_battlePointUIDataList.push_back(battlePointUI);
}

void MiniMap::CalcBattlePointUIPos()
{
	//プレイヤーアイコンの向き設定
	//UIに適応する回転のためプレイヤーのY軸回転をZ軸の回転として求める
	Vector3 playerForward = m_player->GetForward();
	playerForward.y = playerForward.z;
	playerForward.z = 0.0f;

	Quaternion playerIconRot = Quaternion::Identity; 
	playerIconRot.SetRotation(Vector3::AxisY, playerForward);

	m_playerIcon->m_transform.m_localRotation = playerIconRot;

	//バトルポイントアイコンのの位置を設定
	for (auto& battlePointUIData : m_battlePointUIDataList)
	{
		//プレイヤーからバトルポイントへのベクトルを計算
		Vector3 playerToBattlePoint = battlePointUIData.battleAreaPos - m_player->GetPosition();

		playerToBattlePoint.y = 0.0f;
		playerToBattlePoint = playerToBattlePoint / 30.0f;

		if (playerToBattlePoint.LengthSq() > 128 * 128)
		{
			playerToBattlePoint.Normalize();
			playerToBattlePoint = playerToBattlePoint * 128;
		}
		

		//UIの座標を設定
		battlePointUIData.battlePointIcon->m_transform.m_localPosition = Vector3(playerToBattlePoint.x, playerToBattlePoint.z, 0.0f);
	}

}
