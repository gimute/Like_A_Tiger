#include "stdafx.h"
#include "NormalYakuzaAi.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiIdleState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiTrackingState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiWaitingAttackState.h"

#include "Random.h"

namespace NormalYakuzaAiAttackConstant
{
	const float ATTACK_START_RADIUS = 50.0f;
}

//AttackState

void NormalYakuzaAiAttackState::OnEnter()
{
	m_hasStateMachine->SetIsAimMove(true);
	//AIステートを攻撃中に設定
	m_owner->SetAiState(YakuzaAiState::en_YakuzaAiState_AttackReady);
	//集団制御用の役割を攻撃中役割に変更
	m_owner->SetYakuzaRole(YakuzaGroupeRole::en_YakuzaRole_Attacking);
}

void NormalYakuzaAiAttackState::OnUpdate()
{
	//もうすでに攻撃範囲に入っていたらそのまま処理
	//入っていなければ接近処理を行う
	if (m_isInAttackDis || 
		ShouldApproachForAttack())
	{

		m_owner->SetAiState(YakuzaAiState::en_YakuzaAiState_Attacking);
		//攻撃範囲に入っているのでフラグを立てて、攻撃終了までは処理を続行させる
		m_isInAttackDis = true;

		//攻撃処理
		PerformAttack();
	}
}

bool NormalYakuzaAiAttackState::ShouldApproachForAttack()
{	
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


	//攻撃可能範囲に入ったら
	if (toTargetDist < NormalYakuzaAiAttackConstant::ATTACK_START_RADIUS)
	{
		//攻撃範囲に入っているので移動処理はせず、trueを返す
		return true;
	}
	else
	{
		//攻撃範囲に入っていないので移動処理を行い、falseを返す
		moveVec = toTargetDire;

		m_hasStateMachine->SetMoveVec(moveVec);
		return false;
	}
}

void NormalYakuzaAiAttackState::PerformAttack()
{
	//アタックステート初期化
	m_hasStateMachine->ResetAttackFlagsMachine();
	//攻撃用のステートマシン
	auto attackState = m_hasStateMachine->GetAttackStateMachine();
	//コンボ用
	int index = 0;

	//攻撃が終了したら終了処理
	if (!m_attackEndFlag &&
		attackState->GetIsAttackEnds())
	{
		m_isInAttackDis = false;

		m_attackEndFlag = true;

		m_owner->SetAiState(YakuzaAiState::en_YakuzaAiState_WaitMove);

		return;
	}

	//初回攻撃を行う
	if (!m_hasStateMachine->GetAttackFlag() &&
		m_attackEndFlag)
	{
		ComboChoice();

		if (!GetCombo(index))
		{
			return;
		}

		if (index == AttackCommand::en_normalAttack)
		{
			m_hasStateMachine->SetAttackFlag(true);
		}
		else if (index == AttackCommand::en_finishBrow)
		{
			m_hasStateMachine->SetFinishBrowFlag(true);
		}

		m_attackEndFlag = false;

		return;
	}

	//コンボ攻撃を行う
	if (!attackState->GetIsNextCombo() &&
		!m_attackEndFlag)
	{
		if (!GetCombo(index))
		{
			return;
		}

		if (index == AttackCommand::en_normalAttack)
		{
			m_hasStateMachine->SetAttackFlag(true);
		}
		else if (index == AttackCommand::en_finishBrow)
		{
			m_hasStateMachine->SetFinishBrowFlag(true);
		}
	}
}

void NormalYakuzaAiAttackState::OnExit()
{
	//アタックステート内で攻撃終了を伝える
	m_attackEndFlag = true;
	//接近フラグをリセット
	m_isInAttackDis = false;
	//狙い移動を停止
	m_hasStateMachine->SetIsAimMove(false);
	//集団制御用の役割を攻撃終了役割に変更
	m_owner->SetYakuzaRole(YakuzaGroupeRole::en_YakuzaRole_AttackEnd);
	//ステート側に攻撃終了を伝える
	m_hasStateMachine->ResetAttackFlagsMachine();
}

//StateMachine

AiAutoRegister<NormalYakuzaAi> NormalYakuzaAi::aiSet{ EnemyYakuzaType::en_normalYakuza };

IStateBase* NormalYakuzaAi::GetNextState()
{
	//ダメージを受けている最中は操作不可
	if (m_hasStateMachine->GetIsDamage())
	{
		//集団制御用の役割をダメージを受けている役割に変更
		m_yakuzaRole = YakuzaGroupeRole::en_YakuzaRoleHitDamage;
		//AIステートを待ち移動に設定
		m_aiState = YakuzaAiState::en_YakuzaAiState_WaitMove;
		//とりあえずダメージ終了まで待機
		return FindClassNameState<EnemyAiIdleState>();
	}

	if (CanChangeAttack())
	{
		return FindClassNameState<NormalYakuzaAiAttackState>();
	}

	if (CanChangeWaitingAttack())
	{
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

	if (m_aiState == YakuzaAiState::en_YakuzaAiState_WaitMove)
	{
		return true;
	}

	return false;
}

bool NormalYakuzaAi::CanChangeAttack()
{
	if (!m_isInBattle)
	{
		return false;
	}

	if (IsYakuzaAiStateAttack() ||
		m_yakuzaRole == YakuzaGroupeRole::en_YakuzaRole_AttackReady)
	{
		return true;
	}

	return false;
}
