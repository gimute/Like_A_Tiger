#include "stdafx.h"
#include "MiniMap.h"

#include "Battle\BattleAreaManager.h"
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
	CalcBattleAreaUIPos();

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

void MiniMap::ButtleAreaDataUpdate()
{
	//バトルエリアの座標を取得
	auto battleAreaPosList = BattleAreaManager::GetInstance()->GetBattleAreaPositions();

	
	//バトルエリアの座標をUIに設定していく

	//更新し終わったUIのカウント
	int count = 0;

	//すでにある分のバトルエリアアイコンの情報を更新
	for (auto& battlePointData : m_battleAreaUIDataList)
	{
		if (battleAreaPosList.size() > count)
		{
			battlePointData.battleAreaPos = battleAreaPosList[count];
			count++;
		}
		else
		{
			//余ったバトルエリアアイコンを消す
			for (count; m_battleAreaUIDataList.size() > count;)
			{
				//キャンバスからUIを削除
				m_canvas->DeleteUI(m_battleAreaUIDataList.back().battleAreaIcon);
				//配列の要素数を減らす
				m_battleAreaUIDataList.pop_back();
			}
			break;
		}
	}

	//バトルエリアアイコンの数が足りていなければ追加
	for (count; count < battleAreaPosList.size(); count++)
	{
		BattleAreaUIData battleAreaUI;

		battleAreaUI.battleAreaIcon = m_canvas->CreateUI<UIImage>();
		battleAreaUI.battleAreaIcon->Init(m_enemyIconData.imageFilePath.c_str(), m_enemyIconData.iconSize, m_enemyIconData.iconSize);

		battleAreaUI.battleAreaPos = battleAreaPosList[count];

		m_battleAreaUIDataList.push_back(battleAreaUI);
	}
}

void MiniMap::CalcBattleAreaUIPos()
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
	for (auto& battlePointUIData : m_battleAreaUIDataList)
	{
		//プレイヤーからバトルポイントへのベクトルを計算
		Vector3 playerToBattleArea = battlePointUIData.battleAreaPos - m_player->GetPosition();

		playerToBattleArea.y = 0.0f;
		playerToBattleArea = playerToBattleArea / 30.0f;

		if (playerToBattleArea.LengthSq() > 128 * 128)
		{
			playerToBattleArea.Normalize();
			playerToBattleArea = playerToBattleArea * 128;
		}
		

		//UIの座標を設定
		battlePointUIData.battleAreaIcon->m_transform.m_localPosition = Vector3(playerToBattleArea.x, playerToBattleArea.z, 0.0f);
	}

}
