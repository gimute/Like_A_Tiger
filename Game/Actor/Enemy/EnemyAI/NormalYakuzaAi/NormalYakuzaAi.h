#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"

class EnemyAiIdleState;
class EnemyAiTrackingState;

class NormalYakuzaAi : public IEnemyAi
{
public:
	NormalYakuzaAi(YakuzaStateMachine* controllStateMachine) : IEnemyAi(controllStateMachine)
	{
		AddState<EnemyAiIdleState>(controllStateMachine,this);
		AddState<EnemyAiTrackingState>(controllStateMachine,this);

		InitStateMachineClassName<EnemyAiIdleState>();
	}
	//次のステートを取得
	IStateBase* GetNextState() override;
	//追跡状態に移行できるかどうか
	bool CanChangeTraking();
private:
	static AiAutoRegister<NormalYakuzaAi> aiSet;
};

