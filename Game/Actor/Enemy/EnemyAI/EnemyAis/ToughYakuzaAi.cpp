#include "stdafx.h"
#include "ToughYakuzaAi.h"

#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiIdleState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiTrackingState.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\EnemyAiWaitingAttackState.h"

namespace ToughYakuzaAiAttackConstant
{
	const float ATTACK_START_RADIUS = 50.0f;

	const float GUARD_COOL_TIME = 2.0f;

	const float GUARD_CONTINUE_TIME = 3.0f;
}

//AttackState

void ToughYakuzaAiAttackState::OnEnter()
{
	m_hasStateMachine->SetIsAimMove(true);
	//AIステートを攻撃移動中に設定
	m_owner->SetAiState(YakuzaAiState::en_YakuzaAiState_AttackReady);
	//集団制御用の役割を攻撃中役割に変更
	m_owner->SetYakuzaRole(YakuzaGroupeRole::en_YakuzaRole_Attacking);
}

void ToughYakuzaAiAttackState::OnUpdate()
{
	//もうすでに攻撃範囲に入っていたらそのまま処理
	//入っていなければ接近処理を行う
	if (m_isInAttackDis ||
		ShouldApproachForAttack())
	{
		//攻撃中に設定
		m_owner->SetAiState(YakuzaAiState::en_YakuzaAiState_Attacking);
		//攻撃範囲に入っているのでフラグを立てて、攻撃終了までは処理を続行させる
		m_isInAttackDis = true;
		//攻撃処理
		PerformAttack();
	}
}

bool ToughYakuzaAiAttackState::ShouldApproachForAttack()
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
	if (toTargetDist < ToughYakuzaAiAttackConstant::ATTACK_START_RADIUS)
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

void ToughYakuzaAiAttackState::PerformAttack()
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

void ToughYakuzaAiAttackState::OnExit()
{
	//アタックステート内で攻撃終了を伝える
	m_attackEndFlag = true;
	//接近フラグをリセット
	m_isInAttackDis = false;
	//狙い移動を停止
	m_hasStateMachine->SetIsAimMove(false);
	//集団制御用の役割を攻撃終了役割に変更
	m_owner->SetYakuzaRole(YakuzaGroupeRole::en_YakuzaRole_AttackEnd);
	//ガードクールタイムをリセット
	m_owner->SetGaurdCoolTime(ToughYakuzaAiAttackConstant::GUARD_COOL_TIME);
	//ステート側に攻撃終了を伝える
	m_hasStateMachine->ResetAttackFlagsMachine();
}

//GuardState

void ToughYakuzaAiGuardState::OnEnter()
{
	//AIステートをガードに設定
	m_owner->SetAiState(ToughYakuzaUniqueState::en_guard);
	//集団制御用の役割を特殊行動中役割に変更
	m_owner->SetYakuzaRole(YakuzaGroupeRole::en_YakuzaRoleUniqueMoveing);

	//ターゲット方向に防御させる
	Vector3 targetPos = m_owner->GetTargetView().m_targetPosition;
	Vector3 toTargetVec = targetPos - m_hasStateMachine->GetHasCharactarPos();
	toTargetVec.Normalize();

	//防御行動
	//ガード継続時間を設定
	m_guardContinueTime = ToughYakuzaAiAttackConstant::GUARD_CONTINUE_TIME;
	//移動ベクトルを設定
	m_hasStateMachine->SetMoveVec(toTargetVec);
	//防御フラグを立てる
	m_hasStateMachine->SetDefenseFlag(true);
}

void ToughYakuzaAiGuardState::OnUpdate()
{
	//ガード継続時間が終了したらガード解除
	m_guardContinueTime -= g_gameTime->GetFrameDeltaTime();

	if (m_guardContinueTime <= 0.0f)
	{
		m_owner->SetAiState(YakuzaAiState::en_YakuzaAiState_WaitMove);
	}
}

void ToughYakuzaAiGuardState::OnExit()
{
	m_hasStateMachine->SetDefenseFlag(false);

	m_owner->SetAiState(YakuzaAiState::en_YakuzaAiState_WaitMove);

	m_owner->SetYakuzaRole(YakuzaGroupeRole::en_yakuzaRole_AttackWait);

	m_owner->SetGaurdCoolTime(ToughYakuzaAiAttackConstant::GUARD_COOL_TIME);
}

AiAutoRegister<ToughYakuzaAi> ToughYakuzaAi::aiSet{ EnemyYakuzaType::en_toughYakuza };

IStateBase* ToughYakuzaAi::GetNextState()
{
	//掴まれている最中は操作不可
	if (m_hasStateMachine->GetIsGrabBed())
	{
		m_yakuzaRole = YakuzaGroupeRole::en_YakuzaRoleGrabBed;
		//AIステートを待ち移動に設定
		m_aiState = YakuzaAiState::en_YakuzaAiState_WaitMove;
		//ガードクールタイムをリセット
		SetGaurdCoolTime(ToughYakuzaAiAttackConstant::GUARD_COOL_TIME);
		//とりあえず掴み終了まで待機
		return FindClassNameState<EnemyAiIdleState>();
	}

	//ダメージを受けている最中は操作不可
	if (m_hasStateMachine->GetIsDamage())
	{
		//集団制御用の役割をダメージを受けている役割に変更
		m_yakuzaRole = YakuzaGroupeRole::en_YakuzaRoleHitDamage;
		//AIステートを待ち移動に設定
		m_aiState = YakuzaAiState::en_YakuzaAiState_WaitMove;
		//ガードクールタイムをリセット
		SetGaurdCoolTime(ToughYakuzaAiAttackConstant::GUARD_COOL_TIME);
		//とりあえずダメージ終了まで待機
		return FindClassNameState<EnemyAiIdleState>();
	}

	m_guardCoolTime -= g_gameTime->GetFrameDeltaTime();

	//ガード
	if (CanChangeGuard())
	{
		return FindClassNameState<ToughYakuzaAiGuardState>();
	}

	//攻撃
	if (CanChangeAttack())
	{
		return FindClassNameState<ToughYakuzaAiAttackState>();
	}

	//攻撃待機状態
	if (CanChangeWaitingAttack())
	{
		return FindClassNameState<EnemyAiWaitingAttackState>();
	}

	return FindClassNameState<EnemyAiIdleState>();
}

bool ToughYakuzaAi::CanChangeWaitingAttack()
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

bool ToughYakuzaAi::CanChangeAttack()
{
	if (!m_isInBattle)
	{
		return false;
	}

	//攻撃継続条件
	if (IsYakuzaAiStateAttack())
	{
		return true;
	}

	//攻撃開始条件
	if (m_yakuzaRole == YakuzaGroupeRole::en_YakuzaRole_AttackReady &&
		m_aiState != ToughYakuzaUniqueState::en_guard)
	{
		return true;
	}

	return false;
}

bool ToughYakuzaAi::CanChangeGuard()
{
	if (!m_isInBattle ||
		m_aiState == YakuzaAiState::en_YakuzaAiState_Attacking)
	{
		return false;
	}

	if (m_aiState == ToughYakuzaUniqueState::en_guard)
	{
		return true;
	}

	if (m_guardCoolTime > 0.0f)
	{
		return false;
	}

	//条件計算
	//現在のエネミーターゲットビューを取得
	auto targetView = GetTargetView();
	//自身からターゲットに伸びるベクトルを作る
	Vector3 toTargetVec = targetView.m_targetPosition - m_hasStateMachine->GetHasCharactarPos();
	//距離
	float len = toTargetVec.Length();

	if (len >= 80.0f)
	{
		return false;
	}

	Vector3 iFoward = m_hasStateMachine->GetHasCharactarForward();
	iFoward *= -1.0f;

	float dot = iFoward.Dot(targetView.m_targetForward);

	if (dot < 0.3f)
	{
		return false;
	}

	if (targetView.m_isTargetAttacking)
	{
		return true;
	}

	return false;
}