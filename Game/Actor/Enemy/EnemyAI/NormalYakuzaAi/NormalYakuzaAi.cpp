#include "stdafx.h"
#include "NormalYakuzaAi.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiIdleState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiTrackingState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiWaitingAttackState.h"

#include "Random.h"

//定数等
namespace NormalYakuzaAiConstant
{
	const float START_TRACKING_RADIUS = 700.0f;

	const float WAITING_ATTACK_RADIUS = 200.0f;

	const float EXIT_WAITING_ATTACK_RADIUS = 500.0f;

	const float ATTACK_TIME = 3.0f;

	
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
		if (Random::Range(0.9))
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

AiAutoRegister<NormalYakuzaAi> NormalYakuzaAi::aiSet{ EnemyYakuzaType::en_normalYakuza };

IStateBase* NormalYakuzaAi::GetNextState()
{
	//ダメージを受けている最中は操作不可
	if (m_hasStateMachine->GetIsDamage())
	{
		//ダメージ
		m_yakuzaRole = YakuzaRole::en_YakuzaRole_HitDamage;
		//とりあえずダメージ終了まで待機
		return FindClassNameState<EnemyAiIdleState>();
	}

	if (CanChangeAttack())
	{
		return FindClassNameState<NormalYakuzaAiAttackState>();
	}

	if (CanChangeWaitingAttack())
	{
		m_yakuzaRole = m_yakuzaRole = YakuzaRole::en_YakuzaRole_Wait;

		return FindClassNameState<EnemyAiWaitingAttackState>();
	}

	return FindClassNameState<EnemyAiIdleState>();
}

bool NormalYakuzaAi::CanChangeWaitingAttack()
{
	if (!m_isInBattle)
	{
		return false;
	}

	if (m_attackFlag)
	{
		return false;
	}

	return true;
}

bool NormalYakuzaAi::CanChangeAttack()
{
	if (!m_isInBattle)
	{
		return false;
	}

	if (m_yakuzaRole == YakuzaRole::en_YakuzaRole_Attack)
	{
		m_attackFlag = true;

		return true;
	}

	return false;
}

bool NormalYakuzaAi::AttackTimer()
{
	return false;
}
