#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaGenericAttackState.h"

class HirotaYakuzaActionSet : public IYakuzaTypeSet
{
	appState(HirotaYakuzaActionSet)
public:

	enum HirotaYakuzaAnimation
	{

	};

	HirotaYakuzaActionSet() : IYakuzaTypeSet(en_campEnemy)
	{

	}

	void InitStateMachineParam(YakuzaCharacter& useCharacter, YakuzaStateMachine& useStateMachine) override
	{

	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{

	}
private:
	static TypeSetAutoRegister<HirotaYakuzaActionSet> typeSet;
};

