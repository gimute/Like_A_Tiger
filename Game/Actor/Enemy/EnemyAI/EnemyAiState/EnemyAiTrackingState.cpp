#include "stdafx.h"
#include "EnemyAiTrackingState.h"

#include "Actor\Enemy\EnemyAI\IEnemyAi.h"

//TrackingState

void EnemyAiTrackingState::OnEnter()
{

}

void EnemyAiTrackingState::OnUpdate()
{
	auto targetView = m_owner->GetTargetView();

	Vector3 iPosition = m_hasStateMachine->GetHasCharactarPos();

	Vector3 diff = targetView.m_targetPosition - iPosition;

	diff.Normalize();

	m_hasStateMachine->SetMoveVec(diff);
}

void EnemyAiTrackingState::OnExit()
{
	
}