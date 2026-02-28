#include "stdafx.h"
#include "HirotaYakuzaAi.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiIdleState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiTrackingState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiWaitingAttackState.h"

AiAutoRegister<HirotaYakuzaAi> HirotaYakuzaAi::aiSet{ EnemyYakuzaType::en_bossHirotaYakuza };

IStateBase* HirotaYakuzaAi::GetNextState()
{
	if (CanChangeWaitingAttack())
	{
		return FindClassNameState<EnemyAiWaitingAttackState>();
	}

	return FindClassNameState<EnemyAiIdleState>();
}

bool HirotaYakuzaAi::CanChangeWaitingAttack()
{
	if (!m_isInBattle)
	{
		return false;
	}

	if (m_aiState == YakuzaAiState::en_YakuzaAiState_WaitMove)
	{
		return true;
	}

	return false;
}