#pragma once
#include "StateMachineComponents\IStateMachine.h"
#include "StateMachineComponents\IState.h"
#include "Actor\Player\PlayerStates.h"

class PlayerStateMachine : public IStateMachine
{
public:
	//コンストラクタ
	PlayerStateMachine();

	//次のステート値を取得する関数
	IStateBase* GetNextState() override;
};
