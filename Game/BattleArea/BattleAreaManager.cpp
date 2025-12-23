#include "stdafx.h"
#include "BattleAreaManager.h"

BattleAreaManager* BattleAreaManager::m_instance = nullptr;

int BattleAreaManager::CreateArea(
	const Vector3& center,
	float radius
)
{
	//新しい戦闘エリア
	BattleArea newBattleArea(
		m_areaCountNum++,
		center,
		radius
	);
	//新しい戦闘エリア情報
	AreaState newAreaState(
		newBattleArea
	);

	//リスト登録
	m_areas.push_back(newAreaState);

	//Idを返す
	return newAreaState.m_battleArea.m_id;
}

void BattleAreaManager::Update(const Vector3& playerPos)
{
	for (auto& areaPtr : m_areas)
	{
		//プレイヤーがエリア内に入ったかどうか調べる
		bool nowInside = areaPtr.m_battleArea.IsBattleAreaInside(playerPos);

		//もし入ってたら
		if (!areaPtr.m_isPlayerInside && nowInside)
		{
			//通知開始
			for (auto& listener : m_onEnterListeners)
			{
				listener(areaPtr.m_battleArea);
			}
		}

		areaPtr.m_isPlayerInside = nowInside;
	}
}

void BattleAreaManager::RemoveArea(int id)
{
	for (auto areaIt = m_areas.begin();areaIt != m_areas.end();)
	{
		if (areaIt->m_battleArea.m_id == id)
		{
			m_areas.erase(areaIt);
		}
		else
		{
			areaIt++;
		}
	}
}