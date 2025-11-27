#include "stdafx.h"
#include "EnemyAiIdleState.h"

//IdleState

void EnemyAiIdleState::OnEnter()
{

}

void EnemyAiIdleState::OnUpdate()
{
	m_hasStateMachine->SetIsAimMove(false);

	m_hasStateMachine->SetMoveVec(Vector3::Zero);
}

void EnemyAiIdleState::OnExit()
{

}