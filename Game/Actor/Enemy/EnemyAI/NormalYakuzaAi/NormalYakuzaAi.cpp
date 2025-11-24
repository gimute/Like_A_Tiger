#include "stdafx.h"
#include "NormalYakuzaAi.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiIdleState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiTrackingState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiWaitingAttackState.h"

#include "Random.h"

//íËêîìô
namespace WaitingAttackStateConstant
{
	const float TRACKING_START_RADIUS = 600.0f;

	const float WAITING_ATTACK_RADIUS = 400.0f;

	const float EXIT_WAITING_ATTACK_RADIUS = 500.0f;
}

//StateMachine

AiAutoRegister<NormalYakuzaAi> NormalYakuzaAi::aiSet{ EnemyType::en_normalYakuza };

IStateBase* NormalYakuzaAi::GetNextState()
{

	if (CanChangeWaitingAttack())
	{
		return FindClassNameState<EnemyAiWaitingAttackState>();
	}

	if (CanChangeTraking())
	{
		return FindClassNameState<EnemyAiTrackingState>();
	}

	return FindClassNameState<EnemyAiIdleState>();
}

bool NormalYakuzaAi::CanChangeTraking()
{
	Vector3 targetPos = m_targetView.m_targetPosition;

	Vector3 iPos = m_hasStateMachine->GetHasCharactarPos();

	Vector3 targetToIVec = targetPos - iPos;
		
	float radius = WaitingAttackStateConstant::TRACKING_START_RADIUS;

	float radiusSq = radius * radius;

	if (targetToIVec.LengthSq() <= radiusSq)
	{
		return true;
	}
	return false;
}

bool NormalYakuzaAi::CanChangeWaitingAttack()
{

	Vector3 targetPos = m_targetView.m_targetPosition;

	Vector3 iPos = m_hasStateMachine->GetHasCharactarPos();

	Vector3 targetToIVec = targetPos - iPos;

	float radius = 0.0f;

	if (IsNowStateClassName<EnemyAiWaitingAttackState>())
	{
		radius = WaitingAttackStateConstant::EXIT_WAITING_ATTACK_RADIUS;
	}
	else
	{
		radius = WaitingAttackStateConstant::WAITING_ATTACK_RADIUS;
	}

	float radiusSq = radius * radius;


	if (targetToIVec.LengthSq() <= radiusSq)
	{
		return true;
	}
	return false;
}

