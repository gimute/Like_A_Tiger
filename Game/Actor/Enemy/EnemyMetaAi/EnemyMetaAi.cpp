#include "stdafx.h"
#include "EnemyMetaAi.h"

#include <unordered_set>

#include "Actor\Character.h"
#include "Actor\Enemy\EnemyAI\EnemyAiBlackboard .h"

#include "Actor\Enemy\EnemyManager.h"
#include "Actor\Enemy\Enemy.h"

#include "Actor\Enemy\EnemyAI\IEnemyAi.h"
#include "UI/PouseMenuManager.h"

//スタート関数
bool EnemyMetaAi::Start()
{
	return true;
}

void EnemyMetaAi::Update()
{
	if (PouseMenuSceneManager::GetSceneManagerInstance()->IsPoseMenuActive())
	{
		return;
	}

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
	//エネミーの情報リスト
	auto& infoList = EnemyManager::GetInstance()->GetEnemyInfoList();
	//ターゲットビュー
	auto targetView = EnemyManager::GetInstance()->GetTargetView();

	std::unordered_set<int> currentGroupIds;

	for (auto& listPtr : m_metaAiProccesInfoList)
	{
		
		listPtr.m_groupeTargetPosition = targetView.m_targetPosition;

		listPtr.m_camFoward = g_camera3D->GetForward();
	}

	//ここ修正

	//現在の情報リスト分回す
	for (auto& infoPtr : infoList)
	{
		int groupId = infoPtr.m_groupId;
		currentGroupIds.insert(groupId);

		MetaAiProccesInfo* existInfo = nullptr;

		for (auto & metaPtr : m_metaAiProccesInfoList)
		{
			if (metaPtr.m_useGroupeId == groupId)
			{		
				existInfo = &metaPtr;

				break;
			}
		}

		//無いなら新規追加
		if (!existInfo)
		{
			MetaAiProccesInfo newInfo;
			newInfo.m_useGroupeId = groupId;
			m_metaAiProccesInfoList.push_back(newInfo);
		}
	}

	//今フレームに無いGroupIdは削除
	m_metaAiProccesInfoList.erase(
		std::remove_if(
			m_metaAiProccesInfoList.begin(),
			m_metaAiProccesInfoList.end(),
			[&](const MetaAiProccesInfo& meta)
			{
				return currentGroupIds.count(meta.m_useGroupeId) == 0;
			}
		),
		m_metaAiProccesInfoList.end()
	);
}

//状況評価
void EnemyMetaAi::EnemyAiSituationEvaluation()
{
	if (m_metaAiProccesInfoList.empty())
	{
		return;
	}

	for (auto & group : m_metaAiProccesInfoList)
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
	if (m_metaAiProccesInfoList.empty())
	{
		return;
	}

	//グループのリスト数繰り返す
	for (auto & group : m_metaAiProccesInfoList)
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
