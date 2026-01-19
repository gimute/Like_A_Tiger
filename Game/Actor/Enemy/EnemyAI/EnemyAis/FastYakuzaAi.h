#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\IEnemyAttackAiState.h"

class EnemyAiIdleState;
class EnemyAiTrackingState;
class EnemyAiWaitingAttackState;

class FastYakuzaAi : public IEnemyAi
{
	FastYakuzaAi(YakuzaStateMachine* controllStateMachine) : IEnemyAi(controllStateMachine)
	{
		AddState<EnemyAiIdleState>(controllStateMachine, this);
		AddState<EnemyAiTrackingState>(controllStateMachine, this);
		AddState<EnemyAiWaitingAttackState>(controllStateMachine, this);

		InitStateMachineClassName<EnemyAiIdleState>();
	}
	//次のステートを取得
	IStateBase* GetNextState() override;
private:
	//攻撃フラグ
	bool m_attackFlag = false;
public:
private:
	static AiAutoRegister<FastYakuzaAi> aiSet;
};

