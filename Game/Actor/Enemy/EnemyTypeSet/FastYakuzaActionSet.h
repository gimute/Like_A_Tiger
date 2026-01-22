#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
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

class FastYakuzaSecondAttackState : public YakuzaGenericAttackState
{
	appState(FastYakuzaSecondAttackState)
public:
	FastYakuzaSecondAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = FastYakuzaSecondAttackState::ID();
	}
	~FastYakuzaSecondAttackState() = default;
};

class FastYakuzaThirdAttackState : public YakuzaGenericAttackState
{
	appState(FastYakuzaThirdAttackState)
public:
	FastYakuzaThirdAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = FastYakuzaThirdAttackState::ID();
	}
	~FastYakuzaThirdAttackState() = default;
};

class FastYakuzaFourthAttackState : public YakuzaGenericAttackState
{
	appState(FastYakuzaFourthAttackState)
public:
	FastYakuzaFourthAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = FastYakuzaFourthAttackState::ID();
	}
	~FastYakuzaFourthAttackState() = default;
};

class FastYakuzaFirstFinalBlowState : public YakuzaGenericAttackState
{
	appState(FastYakuzaFirstFinalBlowState)
public:
	FastYakuzaFirstFinalBlowState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = FastYakuzaFirstFinalBlowState::ID();
	}
	~FastYakuzaFirstFinalBlowState() = default;
};

class FastYakuzaSecondFinalBlowState : public YakuzaGenericAttackState
{
	appState(FastYakuzaSecondFinalBlowState)
public:
	FastYakuzaSecondFinalBlowState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = FastYakuzaSecondFinalBlowState::ID();
	}
	~FastYakuzaSecondFinalBlowState() = default;
};

class FastYakuzaActionSet : public IYakuzaTypeSet
{
	appState(FastYakuzaActionSet)
public:

	enum FastYakuzaAnimation
	{
		en_crossPunch_1_R = YakuzaAnimation::en_num,
		en_punch_R,
		en_punch_L, 
		en_Uppercut,
		en_kick_R
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
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/Punch_R_1.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/Punch_L_1.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/Uppercut_1.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Nonpbr/Animation/kick_R.tka",false });

		//パラメータ初期化
		ParameterManager::GetInstance().LoadParameter<YakuzaParamater>("Assets/Json/FastYakuzaStatus.json", [](const nlohmann::json& j, YakuzaParamater& p)
			{
				p.maxHP = j["hp"].get<int>();
				p.moveSpeed = j["moveSpeed"].get<float>();
				p.dadgeSpeed = j["dodgeSpeed"].get<float>();
				p.dadgeAnimSpeed = j["dodgeAnimSpeed"].get<float>();
			}
		);
	}

	void InitStateMachineParam(YakuzaCharacter& useCharacter,YakuzaStateMachine& useStateMachine)
	{
		auto param = ParameterManager::GetInstance().GetParameter<YakuzaParamater>();

		useCharacter.SetHP(param->maxHP);

		useStateMachine.SetSwaySpeed(param->dadgeSpeed, param->dadgeAnimSpeed);

		useStateMachine.SetMoveSpeed(param->moveSpeed);
	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{
		AddAttackState<FastYakuzaFirstAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,FastYakuzaSecondAttackState::ID(),0,en_punch_L,250.0f,1.5f},
			{ 5.0f,150.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindLigthA });
		AddAttackState<FastYakuzaSecondAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,FastYakuzaThirdAttackState::ID() ,FastYakuzaSecondFinalBlowState::ID(),en_punch_R,250.0f,1.5f},
			{ 5.0f,150.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindLigthA });
		AddAttackState<FastYakuzaThirdAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,FastYakuzaFourthAttackState::ID(),0,en_punch_L,250.0f,1.5f},
			{ 5.0f,150.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindLigthA });
		AddAttackState<FastYakuzaFourthAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,0,FastYakuzaFirstFinalBlowState::ID(),en_punch_R,250.0f,1.5f},
			{ 5.0f,150.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindLigthA });
		AddAttackState<FastYakuzaFirstFinalBlowState>(
			{ useAttackStateMachine,m_yakuzaCamp,0,0,en_Uppercut,250.0f,1.5f },
			{ 60.0f,150.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA });
		AddAttackState<FastYakuzaSecondFinalBlowState>(
			{ useAttackStateMachine,m_yakuzaCamp,0,0,en_kick_R,250.0f,1.5f },
			{ 25.0f,150.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA });
	}

private:
	static TypeSetAutoRegister<FastYakuzaActionSet> typeSet;
};

