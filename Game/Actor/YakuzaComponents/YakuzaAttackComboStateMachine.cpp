#include "stdafx.h"
#include "YakuzaAttackComboStateMachine.h"

#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

IStateBase* YakuzaAttackComboStateMachine::GetNextState()
{
	uint32_t nextCombo = 0;

	if (GetIsUnderFirstAttack())
	{
		if (m_hasStateMachine->GetAttackFlag())
		{
			nextCombo = GetFirstAttack();
		}
		else if (m_hasStateMachine->GetFinishBrowFlag())
		{
			nextCombo = GetFirstFinishBrow();
		}
	}
	else
	{
		nextCombo = GetNextCombo();
	}


	//次のコンボのハッシュ値を探索
	return FindClassUINT32TState(nextCombo);
}

YakuzaStateMachine* YakuzaAttackComboStateMachine::GetYakuzaStateMachine()
{
	return m_hasStateMachine;
}