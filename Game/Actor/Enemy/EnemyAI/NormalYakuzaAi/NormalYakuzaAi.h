#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"

class NormalYakuzaAi : public IEnemyAi
{
public:
	NormalYakuzaAi(YakuzaStateMachine* controllStateMachine) : IEnemyAi(controllStateMachine) {}
	//次のステートを取得
	IStateBase* GetNextState() override;
private:
	static AiAutoRegister<NormalYakuzaAi> aiSet;
};

