#include "stdafx.h"
#include "BattleAreaManager.h"

#include "Actor\Enemy\EnemyManager.h"
#include "Battle\BattleManager.h"
#include "UI/EnemyIntroduction.h"
#include "Sound\SoundId.h"

BattleAreaManager* BattleAreaManager::m_instance = nullptr;

int BattleAreaManager::CreateArea(
	const Vector3& center,
	float radius,
	EnemyYakuzaType type
)
{
	m_enemyIntroduction = NewGO<EnemyIntroduction>(0, "enemyIntroducation");

	//新しい戦闘エリア
	BattleArea newBattleArea(
		m_areaCountNum++,
		center,
		radius,
		type
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

			//バトルマネージャーにも通知
			NotifyOnBattleManagerEnter(areaPtr);

			//敵の紹介演出アニメーション再生
			m_enemyIntroduction->PlayAnimation();
			//敵、Playerの動きを止めたい
		}

		areaPtr.m_isPlayerInside = nowInside;
	}
}

std::vector<Vector3> BattleAreaManager::GetBattleAreaPositions()
{
	std::vector<Vector3> posList;

	for (auto area : m_areas)
	{
		posList.push_back(area.m_battleArea.m_areaCenter);
	}

	return posList;
}

void BattleAreaManager::RemoveArea(int id)
{
	for (auto areaIt = m_areas.begin();areaIt != m_areas.end();)
	{
		if (areaIt->m_battleArea.m_id == id)
		{
			areaIt = m_areas.erase(areaIt);
		}
		else
		{
			areaIt++;
		}
	}
}

void BattleAreaManager::ResetAreaManager()
{
	m_areaCountNum = 0;

	m_onEnterListeners.clear();
	m_areas.clear();
}

void BattleAreaManager::NotifyOnBattleManagerEnter(AreaState areaState)
{
	auto& enemyInfoList = EnemyManager::GetInstance()->GetEnemyInfoList();
	//戦闘に参加する敵グループ情報
	EnemyInfoGroupe* enemyGroupeInfoPtr = nullptr;

	for (auto& enemyGroupeInfo : enemyInfoList)
	{
		if (enemyGroupeInfo.m_battleAreaId == areaState.m_battleArea.m_id)
		{
			//BattleManagerに戦闘状態にするように指示
			enemyGroupeInfoPtr = &enemyGroupeInfo;

			break;
		}
	}

	BattleManager::GetInstance()->StartBattle(
		BattleInfo(enemyGroupeInfoPtr,&areaState.m_battleArea)
	);
}

