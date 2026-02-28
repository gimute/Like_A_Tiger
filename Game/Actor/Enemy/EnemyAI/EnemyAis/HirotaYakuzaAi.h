#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\IEnemyAttackAiState.h"

class EnemyAiIdleState;
class EnemyAiTrackingState;
class EnemyAiWaitingAttackState;

class HirotaYakuzaAi : public IEnemyAi
{
public:

	HirotaYakuzaAi(YakuzaStateMachine* controllStateMachine) : IEnemyAi(controllStateMachine)
	{
		AddState<EnemyAiIdleState>(controllStateMachine, this);
		AddState<EnemyAiTrackingState>(controllStateMachine, this);
		AddState<EnemyAiWaitingAttackState>(controllStateMachine, this);

		InitStateMachineClassName<EnemyAiIdleState>();
	}
	//次のステートを取得
	IStateBase* GetNextState() override;
private:
	//攻撃待機状態に移行できるかどうか
	bool CanChangeWaitingAttack();

	static AiAutoRegister<HirotaYakuzaAi> aiSet;
};

