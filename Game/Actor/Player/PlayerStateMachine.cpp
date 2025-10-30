#include "stdafx.h"
#include "PlayerStateMachine.h"

#include "PlayerStates.h"
#include "Player.h"

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
	if (m_stickAmountL > 0.01f)
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

void PlayerStateMachine::PlayerPlayAnimation(int animationNum,float interpolateTime)
{
	m_player->GetModelRender()->PlayAnimation(animationNum, interpolateTime);
}

