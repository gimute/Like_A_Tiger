#include "stdafx.h"
#include "EnemyMetaAi.h"

#include <unordered_set>

#include "Actor\Character.h"
#include "Actor\Enemy\EnemyAI\EnemyAiBlackboard .h"

#include "Actor\Enemy\EnemyManager.h"
#include "Actor\Enemy\Enemy.h"

#include "Actor\Enemy\EnemyAI\IEnemyAi.h"

//スタート関数
bool EnemyMetaAi::Start()
{
	return true;
}

void EnemyMetaAi::Update()
{
	//1.情報収集
	EnemyAiDataCollect();
	//2.状況評価
	EnemyAiSituationEvaluation();
	//3.実行処理決定
	ProcessingDecision();
}

//情報収集
void EnemyMetaAi::EnemyAiDataCollect()
{
	auto& enemyGroupList = EnemyManager::GetInstance()->GetEnemyGroupList();
	auto& enemyPairList = EnemyManager::GetInstance()->GetEnemyPairList();

	if (enemyGroupList.empty())
	{
		m_enemyAiInfoGroupeList.clear();
		return;
	}

	auto targetView = EnemyManager::GetInstance()->GetTargetView();
	m_targetPosition = targetView.m_targetPosition;
	m_cameraFoward = g_camera3D->GetForward();

	std::unordered_set<int> currentGroupIds;

	for (int groupId = 0;groupId < enemyGroupList.size(); ++groupId)
	{
		auto& group = enemyGroupList[groupId];
		currentGroupIds.insert(groupId);

		//グループがすでにあるかを探索
		EnemyAiInfoGroupe* existGroup = nullptr;

		for (auto& g : m_enemyAiInfoGroupeList)
		{
			if (g.m_groupId == groupId)
			{
				existGroup = &g;

				break;
			}
		}
		
		//無いなら新規追加
		if (!existGroup)
		{
			EnemyAiInfoGroupe newGroup;
			newGroup.m_groupId = groupId;
			m_enemyAiInfoGroupeList.push_back(newGroup);
			existGroup = &m_enemyAiInfoGroupeList.back();
 		}

		//グループ内部の更新
		existGroup->m_enemyAiInfoList.clear();
		existGroup->m_groupeTargetPosition = m_targetPosition;
		existGroup->m_camFoward = m_cameraFoward;

		//EnemyIdごとにEnemyMemberInfowo作成
		for (auto& id : group.m_enemyID)
		{
			EnemyPair* pair = nullptr;

			for (auto & enemy : enemyPairList)
			{
				if (enemy.m_enemyID == id)
				{
					pair = &enemy;

					break;
				}
			}

			if (!pair)
			{
				continue;
			}

			IEnemyAi* inst = pair->m_enemyAi.get();
			EnemyType type = pair->m_type;
			Vector3 enemyPos = pair->m_enemy->GetPosition();
			bool isInBattle = pair->m_enemyAi.get()->GetIsInBattle();
			bool isActive = true;
			
			EnemyMemberInfo info(
				inst,
				type,
				enemyPos,
				isInBattle,
				isActive
			);

			existGroup->m_enemyAiInfoList.emplace_back(info);
		}
	}

	//今フレームに無いGroupIdは削除
	m_enemyAiInfoGroupeList.erase(
		std::remove_if(
			m_enemyAiInfoGroupeList.begin(),
			m_enemyAiInfoGroupeList.end(),
			[&](const EnemyAiInfoGroupe& g)
			{
				return currentGroupIds.count(g.m_groupId) == 0;
			}
		),
		m_enemyAiInfoGroupeList.end()
	);
}

//状況評価
void EnemyMetaAi::EnemyAiSituationEvaluation()
{
	for (auto & group : m_enemyAiInfoGroupeList)
	{
		if(!group.m_useMetaAI ||
			!group.m_useMetaAI->IsReady(&group))
		{
			continue;
		}

		group.m_useMetaAI->AssignRoles(&group);
	}
}

//処理決定
void EnemyMetaAi::ProcessingDecision()
{
	//グループのリスト数繰り返す
	for (auto & group : m_enemyAiInfoGroupeList)
	{
		group.m_prevMetaAI = group.m_useMetaAI;

		//処理数繰り返して条件に合致する処理を適用する
		for (auto& map : m_processList)
		{
			if (map.second.get()->IsApplicable(&group))
			{
				IMetaAiProcess* nextProcess = map.second.get();
				
				//処理が変わったときだけReset
				if (group.m_useMetaAI != nextProcess)
				{
					//適用処理を決定
					group.m_grouoeState.ResetStateForProcess();
					group.m_useMetaAI = nextProcess;
				}
			}
		}
	}
}
