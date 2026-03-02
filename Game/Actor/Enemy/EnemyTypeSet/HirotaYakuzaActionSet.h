#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaGenericAttackState.h"

class HirotaYakuzaFirstAttackState : public YakuzaGenericAttackState
{
	appState(HirotaYakuzaFirstAttackState)
public:
	HirotaYakuzaFirstAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = HirotaYakuzaFirstAttackState::ID();
	}
	~HirotaYakuzaFirstAttackState() = default;
};

class HirotaYakuzaSecondAttackState : public YakuzaGenericAttackState
{
	appState(HirotaYakuzaSecondAttackState)
public:
	HirotaYakuzaSecondAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = HirotaYakuzaSecondAttackState::ID();
	}
	~HirotaYakuzaSecondAttackState() = default;
};

class HirotaYakuzaThirdAttackState : public YakuzaGenericAttackState
{
	appState(HirotaYakuzaThirdAttackState)
public:
	HirotaYakuzaThirdAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = HirotaYakuzaThirdAttackState::ID();
	}
	~HirotaYakuzaThirdAttackState() = default;
};

class HirotaYakuzaFirstFinalBlowState : public YakuzaGenericAttackState
{
	appState(HirotaYakuzaFirstFinalBlowState)
public:
	HirotaYakuzaFirstFinalBlowState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = HirotaYakuzaFirstFinalBlowState::ID();
	}
	~HirotaYakuzaFirstFinalBlowState() = default;
};

class HirotaYakuzaSecondFinalBlowState : public YakuzaGenericAttackState
{
	appState(HirotaYakuzaSecondFinalBlowState)
public:
	HirotaYakuzaSecondFinalBlowState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = HirotaYakuzaSecondFinalBlowState::ID();
	}
	~HirotaYakuzaSecondFinalBlowState() = default;
};

class HirotaYakuzaSpecialChargeState : public IStateBase
{
	appState(HirotaYakuzaSpecialChargeState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
	VolumeAdjustment* m_volumeAdjustment = nullptr;
public:
	HirotaYakuzaSpecialChargeState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~HirotaYakuzaSpecialChargeState() = default;

	uint32_t m_nextComboHash = 0;

	//チャージ時間
	float m_chargeTimer = 0.0f;
	//対象のポインタ
	YakuzaCharacter* m_targetCharacter = nullptr;

	//チャージ音のポインタ
	SoundHandle m_chargeSEHandle;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class HirotaYakuzaSpecialAttackRushState : public IStateBase
{
	appState(HirotaYakuzaSpecialAttackRushState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
	VolumeAdjustment* m_volumeAdjustment = nullptr;
public:
	HirotaYakuzaSpecialAttackRushState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~HirotaYakuzaSpecialAttackRushState() = default;

	Vector3 m_rushDirection = Vector3::Zero;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class HirotaYakuzaActionSet : public IYakuzaTypeSet
{
	appState(HirotaYakuzaActionSet)
public:

	enum HirotaYakuzaAnimation
	{
		en_hookPunch_1_R = YakuzaAnimation::en_num,
		en_punching_1_R,
		en_punching_1_L,
		en_kickLow_1_L,
		en_kickHigh_1_R,
		en_charge_1,
		en_punchRush_1,
	};

	HirotaYakuzaActionSet() : IYakuzaTypeSet(en_campEnemy)
	{
		m_firstAttackID = HirotaYakuzaFirstAttackState::ID();

		m_firstFinishBrowID = HirotaYakuzaSpecialChargeState::ID();

		m_modelFilePath = "C:Assets/modelData/Character/Crypto/Crypto.tkm";

		m_yakuzaName = "HIROTA";

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
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Grab_Idle_B.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Grab_Throw_B.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Grab_Break_B.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Grab_Atk_B.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/BodyHit.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/BackDeath_E.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Punch_Hook_R_1.tka",false });//ここから攻撃モーション
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Punch_R_1.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Punch_L_1.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Kick_Low_L_1.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Kick_High_R_1.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Charge.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Crypto/Animation/Punch_Rush_1.tka",false });

		//パラメーター初期化
		ParameterManager::GetInstance().LoadParameter<HirotaYakuzaParamater>("Assets/Json/HirotaYakuzaStatus.json", [](const nlohmann::json& j, HirotaYakuzaParamater& p)
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
		auto param = ParameterManager::GetInstance().GetParameter<HirotaYakuzaParamater>();

		useCharacter.SetHP(param->maxHP);

		useStateMachine.SetSwaySpeed(param->dadgeSpeed, param->dadgeAnimSpeed);

		useStateMachine.SetAimMoveSpeedRate(param->moveSpeed);
	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{
		//ここに攻撃ステート追加処理を記述
		AddAttackState<HirotaYakuzaFirstAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,HirotaYakuzaSecondAttackState::ID(),HirotaYakuzaFirstFinalBlowState::ID(),en_punching_1_L,60.0f,1.0f},
			{ 10.0f,150.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindLigthA });

		AddAttackState<HirotaYakuzaSecondAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,HirotaYakuzaThirdAttackState::ID(),0,en_punching_1_R,60.0f,1.0f },
			{ 10.0f,150.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindLigthA });

		AddAttackState<HirotaYakuzaThirdAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,0,0,en_hookPunch_1_R,60.0f,1.0f },
			{ 10.0f,150.0f,SoundId::se_hittingHeavyA },
			{ SoundId::se_cuttingWindHeavyA });

		AddAttackState<HirotaYakuzaFirstFinalBlowState>(
			{ useAttackStateMachine,m_yakuzaCamp,0,HirotaYakuzaSecondFinalBlowState::ID(),en_kickLow_1_L,60.0f,1.0f},
			{ 10.0f,150.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindLigthA });

		AddAttackState<HirotaYakuzaSecondFinalBlowState>(
			{ useAttackStateMachine,m_yakuzaCamp,0,0,en_kickHigh_1_R,60.0f,1.0f },
			{ 10.0f,150.0f,SoundId::se_hittingHeavyA },
			{ SoundId::se_cuttingWindLigthA });

		AddAttackState<HirotaYakuzaSpecialChargeState>(useAttackStateMachine,
			{ 0.0f,0.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA });

		AddAttackState<HirotaYakuzaSpecialAttackRushState>(useAttackStateMachine,
			{ 100.0f,500.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA });
	}
private:
	static TypeSetAutoRegister<HirotaYakuzaActionSet> typeSet;
};

