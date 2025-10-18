#include "stdafx.h"
#include "PlayerStateMachine.h"

#include "PlayerStates.h"

//コンストラクタ
PlayerStateMachine::PlayerStateMachine()
{
	AddState<PlayerIdleState>(this);
	AddState<PlayerWalkState>(this);

	ReqestState<PlayerIdleState>();
}

IStateBase* PlayerStateMachine::GetNextState()
{

	if (CanChangeAttack())
	{

	}

	if (CanChangeWalk())
	{
		return FindClassNameState<PlayerWalkState>();
	}

	return FindClassNameState<PlayerIdleState>();
}

bool PlayerStateMachine::CanChangeWalk()
{
	if (m_stickAmount > 0.01f)
	{
		return true;
	}

	return false;
}

bool PlayerStateMachine::CanChangeAttack()
{
	if (m_attackButtonB)
	{
		return true;
	}

	if (m_finishBrowButtonY)
	{
		return true;
	}

	return false;
}