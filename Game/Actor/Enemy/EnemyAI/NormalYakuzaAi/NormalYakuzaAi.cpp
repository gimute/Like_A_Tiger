#include "stdafx.h"
#include "NormalYakuzaAi.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiIdleState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiTrackingState.h"

//定数等
namespace NormalYakuzaAiConstant
{
	const float TRACKING_START_RADIUS = 400.0f;

	const float WAITING_ATTACK_RADIUS = 200.0f;

	const float EXIT_WAITING_ATTACK_RADIUS = 300.0f;
}

//WaitingAttackState

void NormalYakuzaAiWaitingAttackState::OnEnter()
{

}

void NormalYakuzaAiWaitingAttackState::OnUpdate()
{
	//自身の座標
	Vector3 iPos = m_hasStateMachine->GetHasCharactarPos();
	//ターゲットの位置座標
	Vector3 targetPos = m_owner->GetTargetView().m_targetPosition;
	//ターゲットまでのベクトル
	Vector3 toTarget = targetPos - iPos;
	//距離
	float distSq = toTarget.LengthSq();
	//正規化
	toTarget.Normalize();

	float radius = NormalYakuzaAiConstant::TRACKING_START_RADIUS;

	float radiusSq = radius * radius;
	//移動方向
	Vector3 moveVec = Vector3::Zero;
	//前後後退の境界線
	float desiredDistance = radiusSq - 1.0f;
	//対象が近づいてきたら
	if (distSq < desiredDistance - 0.3f)
	{
		//後退方向に
		moveVec = m_hasStateMachine->GetHasCharactarForward() * -1.0f;
	}
	//対象が遠ざかったら
	else if(distSq > desiredDistance + 0.3f)
	{
		//前進方向に
		moveVec = m_hasStateMachine->GetHasCharactarForward();
	}



}

void NormalYakuzaAiWaitingAttackState::OnExit()
{

}

//StateMachine

AiAutoRegister<NormalYakuzaAi> NormalYakuzaAi::aiSet{ EnemyType::en_normalYakuza };

IStateBase* NormalYakuzaAi::GetNextState()
{

	if (CanChangeWaitingAttack())
	{
		return FindClassNameState<NormalYakuzaAiWaitingAttackState>();
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
		
	float radius = NormalYakuzaAiConstant::TRACKING_START_RADIUS;

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

	if (IsNowStateClassName<NormalYakuzaAiWaitingAttackState>())
	{
		radius = NormalYakuzaAiConstant::EXIT_WAITING_ATTACK_RADIUS;
	}
	else
	{
		radius = NormalYakuzaAiConstant::WAITING_ATTACK_RADIUS;
	}

	float radiusSq = radius * radius;


	if (targetToIVec.LengthSq() <= radiusSq)
	{
		return true;
	}
	return false;
}

