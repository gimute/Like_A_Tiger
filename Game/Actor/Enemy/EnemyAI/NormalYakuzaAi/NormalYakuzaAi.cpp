#include "stdafx.h"
#include "NormalYakuzaAi.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiIdleState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiTrackingState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiWaitingAttackState.h"

#include "Random.h"

//定数等
namespace NormalYakuzaAiConstant
{
	const float TRACKING_START_RADIUS = 700.0f;

	const float WAITING_ATTACK_RADIUS = 400.0f;

	const float EXIT_WAITING_ATTACK_RADIUS = 500.0f;

	const float ATTACK_TIME = 5.0f;

	
}

namespace NormalYakuzaAiAttackConstant
{
	const float ATTACK_START_RADIUS = 50.0f;
}

//AttackState

void NormalYakuzaAiAttackState::OnEnter()
{
	m_hasStateMachine->SetIsAimMove(true);
}

void NormalYakuzaAiAttackState::OnUpdate()
{
	//フラグ初期化
	m_hasStateMachine->SetAttackFlag(false);
	m_hasStateMachine->SetFinishBrowFlag(false);
	//計算のための値
	//攻撃用のステートマシン
	auto attackState = m_hasStateMachine->GetAttackStateMachine();
	//ターゲットビュー
	auto targetView = m_owner->GetTargetView();
	//自身の座標
	Vector3 iPosition = m_hasStateMachine->GetHasCharactarPos();
	//ターゲットの座標
	Vector3 targetPosition = targetView.m_targetPosition;
	//ターゲットまでのベクトル
	Vector3 toTargetDiff = targetPosition - iPosition;
	//ターゲットの位置を送る
	m_hasStateMachine->SetAimMoveTargetPos(targetPosition);
	//ターゲットの方向
	Vector3 toTargetDire = toTargetDiff;
	toTargetDire.Normalize();
	//距離
	float toTargetDist = toTargetDiff.Length();
	//移動ベクトル
	Vector3 moveVec = Vector3::Zero;

	//攻撃に向かう
	if (!m_isInAttackDis)
	{
		moveVec = toTargetDire;

		//攻撃可能範囲に入ったら
		if (toTargetDist < NormalYakuzaAiAttackConstant::ATTACK_START_RADIUS)
		{
			m_isInAttackDis = true;
		}

		m_hasStateMachine->SetMoveVec(moveVec);
		return;
	}

	//攻撃が終了したら終了処理
	if (!m_attackEndFlag &&
		attackState->GetIsAttackEnds())
	{
		m_isInAttackDis = false;

		m_attackEndFlag = true;

		m_owner->SetAttackFlag(false);

		return;
	}

	//初回攻撃を行う
	if (!m_hasStateMachine->GetAttackFlag() &&
		m_attackEndFlag)
	{
		m_hasStateMachine->SetAttackFlag(true);

		m_attackEndFlag = false;
		return;
	}

	//コンボ攻撃を行う
	if (!attackState->GetIsNextCombo() &&
		!m_attackEndFlag)
	{
		if (Random::Range(0.5f))
		{
			m_hasStateMachine->SetAttackFlag(true);
		}
		else
		{
			m_hasStateMachine->SetFinishBrowFlag(true);
		}
	}
}

bool NormalYakuzaAiAttackState::ShouldPerformChaseAttack()
{
	return Random::Range(0.5f);
}

void NormalYakuzaAiAttackState::OnExit()
{
	//アタックステート内で攻撃終了を伝える
	m_attackEndFlag = true;
	//狙い移動を停止
	m_hasStateMachine->SetIsAimMove(false);
	//タイマー初期化
    m_owner->SetAttackTimer(NormalYakuzaAiConstant::ATTACK_TIME);
	//ステート側に攻撃終了を伝える
	m_owner->SetAttackFlag(false);
}

//StateMachine

AiAutoRegister<NormalYakuzaAi> NormalYakuzaAi::aiSet{ EnemyType::en_normalYakuza };

IStateBase* NormalYakuzaAi::GetNextState()
{
	//個々の処理微妙なので修正予定
	if (!m_attackFlag)
	{
		//追跡判定のバグ修正
		m_attackFlag = AttackTimer();
	}
	else
	{
		m_attackTestTime = NormalYakuzaAiConstant::ATTACK_TIME;

		m_attackFlag = true;
	}

	if (CanChangeAttack())
	{
		return FindClassNameState<NormalYakuzaAiAttackState>();
	}

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
		
	float radius = NormalYakuzaAiConstant::TRACKING_START_RADIUS;

	float radiusSq = radius * radius;

	float targetToVecLenSq = targetToIVec.LengthSq();

	if (m_yakuzaRole == YakuzaRole::en_YakuzaRole_Traking)
	{
		return true;
	}

	if (targetToVecLenSq <= radiusSq)
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

bool NormalYakuzaAi::CanChangeAttack()
{
	Vector3 targetPos = m_targetView.m_targetPosition;
	Vector3 iPos = m_hasStateMachine->GetHasCharactarPos(); 
	Vector3 targetToIVec = targetPos - iPos; float radius = 600.0f;
	float radiusSq = radius * radius; float targetToVecLenSq = targetToIVec.LengthSq();

	bool inRange = (targetToVecLenSq >= radiusSq);

	if (inRange)
	{ 
		return false;
	} 
	
	if (!m_attackFlag) 
	{ 
		//ここに攻撃終了タイマーを付けるのがいいかも
		//攻撃終了判断を全体管理AIに任せるか個別の敵に任せるか決める。


		return false;
	} 

	return true;
}

bool NormalYakuzaAi::AttackTimer()
{
	if (m_yakuzaRole == YakuzaRole::en_YakuzaRole_Attack)
	{
		return true;
	}

	return false;
}
