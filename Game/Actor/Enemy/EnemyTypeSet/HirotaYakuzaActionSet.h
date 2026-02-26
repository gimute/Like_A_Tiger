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
		m_firstAttackID = 0;

		m_firstFinishBrowID = 0;

		m_modelFilePath = "C:Assets/modelData/Character/Crypto/Crypto.tkm";

		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });
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

