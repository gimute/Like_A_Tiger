#include "stdafx.h"
#include "MiniMap.h"

#include "Actor/Player/Player.h"

MiniMap::MiniMap()
{
	m_canvas = std::make_shared<UICanvas>();

	m_mapImage = m_canvas->CreateUI<UIImage>();
	m_mapImage->Init("Assets/spriteData/MiniMap/MapImage.DDS", 256, 256);

	m_playerIcon = m_canvas->CreateUI<UIImage>();
	m_playerIcon->Init("Assets/spriteData/MiniMap/PlayerIcon.DDS", 10, 10);
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
	battlePointUI.battlePointIcon->Init("Assets/spriteData/MiniMap/EnemyIcon.DDS", 10, 10);

	battlePointUI.battleAreaPos = pos;

	m_battlePointUIDataList.push_back(battlePointUI);
}

void MiniMap::CalcBattlePointUIPos()
{
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
