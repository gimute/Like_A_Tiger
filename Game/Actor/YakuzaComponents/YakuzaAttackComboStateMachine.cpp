#include "stdafx.h"
#include "YakuzaAttackComboStateMachine.h"

#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

//攻撃終了ステート（コンストラクタで初期化時に入れる）

void AttackEndState::OnEnter()
{
	m_owner->SetIsNextCombo(false);
}

void AttackEndState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	stateMachine->SetIsComboTransition(false);

	m_owner->SetIsAttackEnds(true);
}

void AttackEndState::OnExit()
{

}

//攻撃終了時にAttackComboStateMachineを初期化するためのだけのステート

void ResetState::OnEnter()
{

}

void ResetState::OnUpdate()
{

}

void ResetState::OnExit()
{

}

//

IStateBase* YakuzaAttackComboStateMachine::GetNextState()
{
	uint32_t nextStateHush = GetNextCombo();
	IStateBase* nextState = FindClassUINT32TState(nextStateHush);
	
	//現在のステートがリセットだったら今のコンボハッシュ値を変更せずに返す
	if (nextStateHush == ResetState::ID())
	{
		return nextState;
	}

	if (nextStateHush != GetNowCombo())
	{
		SetIsCreateAttackCollision(false);
	}
	else
	{
		if (m_hasStateMachine->IsHasCharacterAttackCollisionActive())
		{
			SetIsCreateAttackCollision(true);
		}
	}

	//次のコンボが設定されていなかったら攻撃終了ステートへ
	if (!nextState)
	{
		SetNowCombo(AttackEndState::ID());

		return FindClassUINT32TState(AttackEndState::ID());
	}

	SetNowCombo(nextStateHush);

	//次のコンボのハッシュ値を探索
	return nextState;
}

void YakuzaAttackComboStateMachine::StartFirstAttack()
{
	SetIsAttackEnds(false);

	InitStateMachineUINT32T(m_firstAttack);
	SetNextCombo(m_firstAttack);
}

void YakuzaAttackComboStateMachine::StartFirstFinishBrow()
{
	SetIsAttackEnds(false);

	InitStateMachineUINT32T(m_firstFinishBrow);
	SetNextCombo(m_firstFinishBrow);
}

void YakuzaAttackComboStateMachine::ResetAttackStateMachine()
{
	SetNextCombo(ResetState::ID());
	
	UpdateStateMachine();
}

YakuzaStateMachine* YakuzaAttackComboStateMachine::GetYakuzaStateMachine()
{
	return m_hasStateMachine;
}


