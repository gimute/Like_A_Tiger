#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

class ToughYakuzaActionSet : public IYakuzaTypeSet
{
	appState(ToughYakuzaActionSet)
public:

	ToughYakuzaActionSet() : IYakuzaTypeSet(en_campEnemy)
	{

	}

	void InitStateMachineParam(YakuzaCharacter& useCharacter, YakuzaStateMachine& useStateMachine) override
	{
		


	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{
		//ここに攻撃ステート追加処理を記述
	}
};

