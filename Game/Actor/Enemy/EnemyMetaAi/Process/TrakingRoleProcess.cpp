#include "stdafx.h"
#include "TrakingRoleProcess.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiTrackingState.h"
#include "Actor\Enemy\EnemyManager.h"

void TrakingRoleProcess::AssignRoles(MetaAiProccesInfo* groupePtr)
{
	auto& enemyInfoList = groupePtr->FindGroup(groupePtr->m_useGroupeId)->m_enemyAiInfoList;

	for (auto& ptr : enemyInfoList)
	{
		ptr.m_enemyAi->SetYakuzaRole(YakuzaRole::en_YakuzaRole_Traking);
	}

	EnemyManager::GetInstance()->SetEnemyGroupeInBattle(groupePtr->m_useGroupeId, true);

	groupePtr->m_grouoeState.m_isTrakingProcessEnd = true;
}

bool TrakingRoleProcess::IsApplicable(MetaAiProccesInfo* groupePtr)
{
	auto& enemyInfoList = groupePtr->FindGroup(groupePtr->m_useGroupeId)->m_enemyAiInfoList;

	//‚·‚Å‚Éí“¬’†‚È‚çˆ—‚µ‚È‚¢
	if (groupePtr->FindGroup(groupePtr->m_useGroupeId)->m_inBattle)
	{
		return false;
	}

	for (auto& ptr : enemyInfoList)
	{
		if (ptr.m_enemyAi->IsAiNowStateClassName<EnemyAiTrackingState>())
		{
			return true;
		}
	}

	return false;
}

bool TrakingRoleProcess::IsReady(MetaAiProccesInfo* groupePtr)
{
	int trakingStateEnemy = 0;
	auto& enemyInfoList = groupePtr->FindGroup(groupePtr->m_useGroupeId)->m_enemyAiInfoList;

	for (auto& ptr : enemyInfoList)
	{
		if (ptr.m_enemyAi->IsAiNowStateClassName<EnemyAiTrackingState>())
		{
			trakingStateEnemy++;
		}
	}	

	if (groupePtr->m_grouoeState.m_isTrakingProcessEnd ||
		trakingStateEnemy >= enemyInfoList.size())
	{
		groupePtr->m_grouoeState.m_isTrakingProcessEnd = false;

		return false;
	}

	return true;
}