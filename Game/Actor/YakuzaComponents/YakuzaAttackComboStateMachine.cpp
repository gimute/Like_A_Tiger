#include "stdafx.h"
#include "YakuzaAttackComboStateMachine.h"

#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

IStateBase* YakuzaAttackComboStateMachine::GetNextState()
{
	//次のコンボのハッシュ値を探索
	return FindClassUINT32TState(GetNextCombo());
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


