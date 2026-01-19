#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"

class FastYakuzaActionSet : public IYakuzaTypeSet
{
	appState(FastYakuzaActionSet)
public:

	FastYakuzaActionSet() : IYakuzaTypeSet(en_campEnemy)
	{
		m_firstAttackID;

		m_modelFilePath = "Assets/modelData/Character/Nonpbr/Nonpbr.tkm";

		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/Idle.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/Walking.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/Running.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/Fighting_Idle.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/ForwardStep.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/BackStep.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/SideStep_Right.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/SideStep_Left.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/Dodge_Forward.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/Dodge_Right.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/Dodge_Left.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/Dodge_Back.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/BodyHit.tka",false });//この種類のエネミーは防御をしないためモーションには仮の物を
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/BodyHit.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/BackDeath_E.tka",false });

	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{

	}

private:
	static TypeSetAutoRegister<FastYakuzaActionSet> typeSet;
};

