#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaGenericAttackState.h"

class ToughYakuzaFirstFinalBlowState : public YakuzaGenericAttackState
{
	appState(ToughYakuzaFirstFinalBlowState)
public:
	ToughYakuzaFirstFinalBlowState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = ToughYakuzaFirstFinalBlowState::ID();
	}
	~ToughYakuzaFirstFinalBlowState() = default;
};

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
		m_firstFinishBrowID = ToughYakuzaFirstFinalBlowState::ID();

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

		//パラメータ初期化
		ParameterManager::GetInstance().LoadParameter<YakuzaParamater>("Assets/Json/ToughYakuzaStatus.json", [](const nlohmann::json& j, YakuzaParamater& p)
			{
				p.maxHP = j["hp"].get<int>();
				p.moveSpeed = j["moveSpeed"].get<float>();
				p.dadgeSpeed = j["dodgeSpeed"].get<float>();
				p.dadgeAnimSpeed = j["dodgeAnimSpeed"].get<float>();
			}
		);
	}

	void InitStateMachineParam(YakuzaCharacter& useCharacter, YakuzaStateMachine& useStateMachine) override
	{
		auto param = ParameterManager::GetInstance().GetParameter<YakuzaParamater>();

		useCharacter.SetHP(param->maxHP);

		useStateMachine.SetSwaySpeed(param->dadgeSpeed, param->dadgeAnimSpeed);

		useStateMachine.SetMoveSpeed(param->moveSpeed);
	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{
		//ここに攻撃ステート追加処理を記述
		AddAttackState<ToughYakuzaFirstFinalBlowState>(
			{ useAttackStateMachine,m_yakuzaCamp,0,0,en_hookPunch_1_R,60.0f,1.0f },
			{ 60.0f,150.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA });
	}
private:
	static TypeSetAutoRegister<ToughYakuzaActionSet> typeSet;
};

