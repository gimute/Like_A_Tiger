#include "stdafx.h"
#include "PlayerStateMachine.h"

#include "PlayerStates.h"

//コンストラクタ
PlayerStateMachine::PlayerStateMachine()
{
	AddState<PlayerIdleState>();
	AddState<PlayerWalkState>();

	ReqestState<PlayerIdleState>();
}

IStateBase* PlayerStateMachine::GetNextState()
{
	return FindClassNameState<PlayerWalkState>();
}