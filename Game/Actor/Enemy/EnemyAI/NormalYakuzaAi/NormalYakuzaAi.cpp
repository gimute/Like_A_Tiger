#include "stdafx.h"
#include "NormalYakuzaAi.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiIdleState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiTrackingState.h"

#include <random>

//定数等
namespace NormalYakuzaAiConstant
{
	const float TRACKING_START_RADIUS = 600.0f;

	const float WAITING_ATTACK_RADIUS = 400.0f;

	const float EXIT_WAITING_ATTACK_RADIUS = 500.0f;

	const float BUFFER = 0.3;

	const float FORWARD_WEIGHT = 0.6;
	const float BACKWARD_WEIGHT = 0.8;

	const float RANDOM_TIME_MAX = 1.5f;
	const float RANDOM_TIME_MIN = 0.5f;
}

//WaitingAttackState

void NormalYakuzaAiWaitingAttackState::OnEnter()
{
	m_hasStateMachine->SetIsAimMove(true);
}

void NormalYakuzaAiWaitingAttackState::OnUpdate()
{
	//移動方向
	Vector3 moveVec = Vector3::Zero;
	//自身の座標
	Vector3 iPos = m_hasStateMachine->GetHasCharactarPos();
	//ターゲットの位置座標
	Vector3 targetPos = m_owner->GetTargetView().m_targetPosition;
	//ターゲットの位置を送る
	m_hasStateMachine->SetAimMoveTargetPos(targetPos);
	//ターゲットまでのベクトル
	Vector3 toTarget = targetPos - iPos;
	//距離
	float dist = toTarget.Length();
	//正面方向
	Vector3 forward = toTarget;
	forward.Normalize();



	//右に移動するか左に移動するか
	Vector3 right = Vector3(toTarget.z,0.0f,-toTarget.x);
	right.Normalize();
	Vector3 left = right * -1.0f;
	m_randomTimer -= g_gameTime->GetFrameDeltaTime();
	if (m_randomTimer <= 0.0f)
	{
		m_isMoveLR = !m_isMoveLR;
		std::random_device rd;            
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> distF(
			NormalYakuzaAiConstant::RANDOM_TIME_MIN,
			NormalYakuzaAiConstant::RANDOM_TIME_MAX
		);
		float v = distF(gen);
		m_randomTimer = v;
		//std::uniform_real_distribution<int> distI(
		//	WaitingMove::en_wait,
		//	WaitingMove::en_rightMove
		//);
		//m_waitingMove = distI(gen);
	}



	if (m_isMoveLR)
	{
		moveVec += right;
	}
	else
	{
		moveVec += left;
	}

	const float desiredDistance = NormalYakuzaAiConstant::WAITING_ATTACK_RADIUS;
	const float buffer = NormalYakuzaAiConstant::BUFFER;

	if (dist < desiredDistance - buffer)
	{
		moveVec -= forward * NormalYakuzaAiConstant::BACKWARD_WEIGHT;
	}
	else
	{
		moveVec += forward * NormalYakuzaAiConstant::FORWARD_WEIGHT;
	}
	moveVec.Normalize();

	m_hasStateMachine->SetMoveVec(moveVec);
}

void NormalYakuzaAiWaitingAttackState::OnExit()
{
	m_hasStateMachine->SetIsAimMove(false);
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

