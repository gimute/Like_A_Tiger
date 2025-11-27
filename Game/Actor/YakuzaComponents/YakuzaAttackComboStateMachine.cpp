#include "stdafx.h"
#include "YakuzaAttackComboStateMachine.h"

#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

//攻撃終了ステート（コンストラクタで初期化時に入れる）

void EnemyAiAttackEndState::OnEnter()
{

}

void EnemyAiAttackEndState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	stateMachine->SetIsComboTransition(false);

	m_owner->SetIsAttackEnds(true);
}

void EnemyAiAttackEndState::OnExit()
{
	m_owner->SetIsNextCombo(false);

	m_owner->SetIsLastCombo(true);
}

//

IStateBase* YakuzaAttackComboStateMachine::GetNextState()
{
	IStateBase* nextState = FindClassUINT32TState(GetNextCombo());

	//次のコンボが設定されていなかったら攻撃終了ステートへ
	if (!nextState)
	{
		return FindClassUINT32TState(EnemyAiAttackEndState::ID());
	}

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


YakuzaStateMachine* YakuzaAttackComboStateMachine::GetYakuzaStateMachine()
{
	return m_hasStateMachine;
}


