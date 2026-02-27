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
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Walking.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Running.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Fighting_Idle.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/ForwardStep.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/BackStep.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/SideStep_Right.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/SideStep_Left.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//この敵は回避動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//この敵は回避動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//この敵は回避動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//この敵は回避動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//この敵は防御動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//この敵は掴み動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//この敵は掴み動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//この敵は掴み動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//この敵は掴み動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//この敵は掴み動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//掴まれは一旦待ち
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//掴まれは一旦待ち
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//掴まれは一旦待ち
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Idle.tka",true });//掴まれは一旦待ち
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/BodyHit.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/BackDeath_E.tka",false });
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

