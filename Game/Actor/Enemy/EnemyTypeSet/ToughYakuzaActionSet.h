#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaGenericAttackState.h"

class ToughYakuzaFirstAttackState : public YakuzaGenericAttackState
{
	appState(ToughYakuzaFirstAttackState)
public:
	ToughYakuzaFirstAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = ToughYakuzaFirstAttackState::ID();
	}
	~ToughYakuzaFirstAttackState() = default;

};

class ToughYakuzaSecondAttackState : public YakuzaGenericAttackState
{
	appState(ToughYakuzaSecondAttackState)
public:
	ToughYakuzaSecondAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = ToughYakuzaSecondAttackState::ID();
	}
	~ToughYakuzaSecondAttackState() = default;
};

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

class ToughYakuzaSecondFinalBlowState : public YakuzaGenericAttackState
{
	appState(ToughYakuzaSecondFinalBlowState)
public:
	ToughYakuzaSecondFinalBlowState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = ToughYakuzaSecondFinalBlowState::ID();
	}
	~ToughYakuzaSecondFinalBlowState() = default;
};

class ToughYakuzaActionSet : public IYakuzaTypeSet
{
	appState(ToughYakuzaActionSet)
public:

	enum ToughYakuzaAnimation
	{
		en_hookPunch_1_R = YakuzaAnimation::en_num,
		en_punching_1_L,
		en_fowardHookPunch_1_R,
		en_heavyPunch_1_L,
	};

	ToughYakuzaActionSet() : IYakuzaTypeSet(en_campEnemy)
	{
		m_firstAttackID = ToughYakuzaFirstAttackState::ID();

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
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/Punching_L.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/FowardHookPunch_R.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Adam/Animation/HeavyPunch_L.tka",false });
		

		//パラメータ初期化
		ParameterManager::GetInstance().LoadParameter<ToughYakuzaParamater>("Assets/Json/ToughYakuzaStatus.json", [](const nlohmann::json& j, ToughYakuzaParamater& p)
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
		auto param = ParameterManager::GetInstance().GetParameter<ToughYakuzaParamater>();

		useCharacter.SetHP(param->maxHP);

		useStateMachine.SetSwaySpeed(param->dadgeSpeed, param->dadgeAnimSpeed);

		useStateMachine.SetMoveSpeed(param->moveSpeed);
	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{
		//ここに攻撃ステート追加処理を記述
		AddAttackState<ToughYakuzaFirstAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,ToughYakuzaSecondAttackState::ID(),0,en_punching_1_L,60.0f,1.0f},
			{ 10.0f,150.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindLigthA });
		AddAttackState<ToughYakuzaSecondAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,0,ToughYakuzaSecondFinalBlowState::ID(),en_fowardHookPunch_1_R,100.0f,1.0f},
			{ 20.0f,150.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindLigthA });
		AddAttackState<ToughYakuzaFirstFinalBlowState>(
			{ useAttackStateMachine,m_yakuzaCamp,0,0,en_hookPunch_1_R,60.0f,1.0f },
			{ 30.0f,300.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA });
		AddAttackState<ToughYakuzaSecondFinalBlowState>(
			{ useAttackStateMachine,m_yakuzaCamp,0,0,en_heavyPunch_1_L,20.0f,1.0f },
			{ 50.0f,150.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA });
	}
private:
	static TypeSetAutoRegister<ToughYakuzaActionSet> typeSet;
};

