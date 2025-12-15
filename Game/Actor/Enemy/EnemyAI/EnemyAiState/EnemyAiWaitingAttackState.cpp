#include "stdafx.h"
#include "EnemyAiWaitingAttackState.h"

#include "Actor\Enemy\EnemyAI\IEnemyAi.h"
#include "Random.h"

//定数等
namespace NormalYakuzaAiConstant
{
	const float MOVE_LIMIT_MAX = 450.0f;
	const float MOVE_LIMIT_MIN = 50.0f;

	const float BUFFER = 0.3;

	const float FORWARD_WEIGHT = 0.6;
	const float BACKWARD_WEIGHT = 0.8;

	const float BACK_TIME = 3.0f;

	const float RANDOM_TIME_MAX = 1.5f;
	const float RANDOM_TIME_MIN = 0.5f;
}

//WaitingAttackState

void EnemyAiWaitingAttackState::OnEnter()
{
	m_hasStateMachine->SetIsAimMove(true);
}

void EnemyAiWaitingAttackState::OnUpdate()
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
	//範囲定数
	const float buffer = NormalYakuzaAiConstant::BUFFER;
	const float limitMax = NormalYakuzaAiConstant::MOVE_LIMIT_MAX;
	const float limitMin = NormalYakuzaAiConstant::MOVE_LIMIT_MIN;

	//範囲外に出ているかどうかの判定
	if (dist > limitMax + buffer ||
		dist < limitMin - buffer ||
		m_isLimitOut
		)
	{

		Vector3 limitOutMoveVec = Vector3::Zero;

		//もし外側に出てしまっているなら
		if (dist > limitMax + buffer && !m_isLimitOut)
		{
			//一定範囲まで内側に来るまで移動
			m_limitOutMoveLine = limitMax - 100.0f;

			//前に進むように
			m_LimitOutFB = true;
			m_isLimitOut = true;
		}
		//もし内側にきすぎているなら
		else if (dist < limitMin - buffer && !m_isLimitOut)
		{
			//一定範囲まで外側に来るように移動
			m_limitOutMoveLine = limitMin + 100.0f;

			//タイマー設定
			m_backTime = NormalYakuzaAiConstant::BACK_TIME;
			//後ろに動くように
			m_LimitOutFB = false;
			m_isLimitOut = true;
		}

		if (m_LimitOutFB)
		{
			limitOutMoveVec += forward * NormalYakuzaAiConstant::FORWARD_WEIGHT;
		}
		else
		{
			if (m_backTime <= 0.0f)
			{
				limitOutMoveVec -= forward * NormalYakuzaAiConstant::FORWARD_WEIGHT;
			}
			else
			{
				m_backTime -= g_gameTime->GetFrameDeltaTime();
			}
		}

		if (m_LimitOutFB &&
			dist < m_limitOutMoveLine ||
			!m_LimitOutFB &&
			dist > m_limitOutMoveLine)
		{
			m_isLimitOut = false;
			m_limitOutMoveLine = 0.0f;
			m_backTime = 0.0f;
		}

		m_hasStateMachine->SetMoveVec(limitOutMoveVec);
		return;
	}

	//右に移動するか左に移動するか
	Vector3 right = Vector3(toTarget.z, 0.0f, -toTarget.x);
	right.Normalize();
	Vector3 left = right * -1.0f;

	m_randomTimer -= g_gameTime->GetFrameDeltaTime();

	//移動範囲外に出ているなら抽選はしない
	if (m_randomTimer <= 0.0f)
	{
		m_randomTimer = Random::Range(
			NormalYakuzaAiConstant::RANDOM_TIME_MIN,
			NormalYakuzaAiConstant::RANDOM_TIME_MAX
		);

		int moveDirMax = WaitingMove::en_rightMove;
		int moveDirMin = 0;

		m_waitingMove = Random::Range(
			moveDirMin,
			moveDirMax
		);
	}

	switch (m_waitingMove)
	{
		break;
	case EnemyAiWaitingAttackState::en_fowardMove:
		moveVec += forward * NormalYakuzaAiConstant::FORWARD_WEIGHT;
		break;
	case EnemyAiWaitingAttackState::en_backMove:
		moveVec -= forward * NormalYakuzaAiConstant::BACKWARD_WEIGHT;
		break;
	case EnemyAiWaitingAttackState::en_leftMove:
		moveVec += left;
		break;
	case EnemyAiWaitingAttackState::en_rightMove:
		moveVec += right;
		break;
	default:
		moveVec = Vector3::Zero;
		break;
	}

	moveVec.Normalize();
	m_hasStateMachine->SetMoveVec(moveVec);
	return;
}

void EnemyAiWaitingAttackState::OnExit()
{
	m_hasStateMachine->SetIsAimMove(false);
}

