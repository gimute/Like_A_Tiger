#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

class ToughYakuzaActionSet : public IYakuzaTypeSet
{
	appState(ToughYakuzaActionSet)
public:

	enum ToughYakuzaAnimation
	{
		en_hookPunch_1_R = YakuzaAnimation::en_num,
	};

	ToughYakuzaActionSet() : IYakuzaTypeSet(en_campEnemy)
	{
		

		m_modelFilePath = "Assets/modelData/Character/Adam/Adam.tkm";

		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Idle.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Walking.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Running.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Fighting_Idle.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/ForwardStep.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/BackStep.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/SideStep_Right.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/SideStep_Left.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/SideStep_Left.tka",true });//この敵は回避動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/SideStep_Left.tka",true });//この敵は回避動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/SideStep_Left.tka",true });//この敵は回避動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/SideStep_Left.tka",true });//この敵は回避動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Guard_Idle_A.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Guard_Idle_A.tka",true });//この敵は掴み動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Guard_Idle_A.tka",true });//この敵は掴み動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Guard_Idle_A.tka",true });//この敵は掴み動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Guard_Idle_A.tka",true });//この敵は掴み動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Guard_Idle_A.tka",true });//この敵は掴み動作をしないため、仮のモーションを入れておく
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Grab_Idle_B.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Grab_Throw_B.tka",false});
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Grab_Break_B.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Grab_Atk_B.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/BodyHit.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/BackDeath_E.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/HookPunch_R.tka",false });//ここから戦闘モーション

	}

	void InitStateMachineParam(YakuzaCharacter& useCharacter, YakuzaStateMachine& useStateMachine) override
	{
		


	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{
		//ここに攻撃ステート追加処理を記述
	}
private:
	static TypeSetAutoRegister<ToughYakuzaActionSet> typeSet;
};

