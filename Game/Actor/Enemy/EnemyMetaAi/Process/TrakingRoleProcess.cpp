#include "stdafx.h"
#include "TrakingRoleProcess.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiTrackingState.h"


void TrakingRoleProcess::AssignRoles(EnemyAiInfoGroupe* groupePtr)
{
	auto enemyInfoList = groupePtr->m_enemyAiInfoList;

	for (auto& ptr : enemyInfoList)
	{
		ptr.m_enemyAi->SetYakuzaRole(YakuzaRole::en_YakuzaRole_Traking);
	}

	groupePtr->m_grouoeState.m_isProcessEnd = true;
}

bool TrakingRoleProcess::IsApplicable(EnemyAiInfoGroupe* groupePtr)
{
	auto enemyInfoList = groupePtr->m_enemyAiInfoList;

	for (auto& ptr : enemyInfoList)
	{
		if (ptr.m_enemyAi->IsAiNowStateClassName<EnemyAiTrackingState>())
		{
			return true;
		}
	}

	return false;
}

bool TrakingRoleProcess::IsReady(EnemyAiInfoGroupe* groupePtr)
{
	int trakingStateEnemy = 0;
	auto enemyInfoList = groupePtr->m_enemyAiInfoList;

	for (auto& ptr : enemyInfoList)
	{
		if (ptr.m_enemyAi->IsAiNowStateClassName<EnemyAiTrackingState>())
		{
			trakingStateEnemy++;
		}
	}	

	if (groupePtr->m_grouoeState.m_isProcessEnd ||
		trakingStateEnemy >= enemyInfoList.size())
	{
		groupePtr->m_grouoeState.m_isProcessEnd = false;

		return false;
	}

	return true;
}