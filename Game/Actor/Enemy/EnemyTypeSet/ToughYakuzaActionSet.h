#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

class ToughYakuzaActionSet : public IYakuzaTypeSet
{
	appState(ToughYakuzaActionSet)
public:

	ToughYakuzaActionSet() : IYakuzaTypeSet(en_campEnemy)
	{
		

		m_modelFilePath = "Assets/modelData/Character/Adam/Adam.tkm";

		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Idle.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Walking.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Running.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Fighting_Idle.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/ForwardStep.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/BackStep.tka",true });
	}

	void InitStateMachineParam(YakuzaCharacter& useCharacter, YakuzaStateMachine& useStateMachine) override
	{
		


	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{
		//ここに攻撃ステート追加処理を記述
	}
};

