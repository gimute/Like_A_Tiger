#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaGenericAttackState.h"

class FastYakuzaFirstAttackState : public YakuzaGenericAttackState
{
	appState(FastYakuzaFirstAttackState)
public:
	FastYakuzaFirstAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = FastYakuzaFirstAttackState::ID();
	}
	~FastYakuzaFirstAttackState() = default;
};

class FastYakuzaActionSet : public IYakuzaTypeSet
{
	appState(FastYakuzaActionSet)
public:

	enum FastYakuzaAnimation
	{
		en_crossPunch_1_R = YakuzaAnimation::en_num,
	};

	FastYakuzaActionSet() : IYakuzaTypeSet(en_campEnemy)
	{
		m_firstAttackID = FastYakuzaFirstAttackState::ID();

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
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/Cross_Punch_1_R.tka",false });

	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{
		AddAttackState<FastYakuzaFirstAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,0,0,en_crossPunch_1_R,50.0f,1.5f},
			{ 10.0f,150.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindLigthA });
	}

private:
	static TypeSetAutoRegister<FastYakuzaActionSet> typeSet;
};

