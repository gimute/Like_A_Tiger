#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaGenericAttackState.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class PlayerFirstAttackState : public YakuzaGenericAttackState
{
	appState(PlayerFirstAttackState)
public:
	PlayerFirstAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = PlayerFirstAttackState::ID();
	}

	~PlayerFirstAttackState() = default;
};

class PlayerSecondAttackState : public YakuzaGenericAttackState
{
	appState(PlayerSecondAttackState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	PlayerSecondAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = PlayerSecondAttackState::ID();
	}


	~PlayerSecondAttackState() = default;
};

class PlayerThirdAttackState : public YakuzaGenericAttackState
{
	appState(PlayerThirdAttackState)
public:
	PlayerThirdAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = PlayerThirdAttackState::ID();
	}

	~PlayerThirdAttackState() = default;
};

class PlayerFourthAttackState : public IStateBase
{
	appState(PlayerFourthAttackState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	PlayerFourthAttackState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~PlayerFourthAttackState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class PlayerZeroFinalBlowState : public YakuzaGenericAttackState
{
	appState(PlayerZeroFinalBlowState)
public:
	PlayerZeroFinalBlowState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = PlayerZeroFinalBlowState::ID();
	}

	~PlayerZeroFinalBlowState() = default;
};

class PlayerFirstFinalBlowState : public IStateBase
{
	appState(PlayerFirstFinalBlowState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	PlayerFirstFinalBlowState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~PlayerFirstFinalBlowState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class PlayerSecondFinalBlowState : public IStateBase
{
	appState(PlayerSecondFinalBlowState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	PlayerSecondFinalBlowState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~PlayerSecondFinalBlowState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class PlayerThirdFinalBlowState : public IStateBase
{
	appState(PlayerThirdFinalBlowState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	PlayerThirdFinalBlowState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~PlayerThirdFinalBlowState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class PlayerFourthFinalBlowState : public IStateBase
{
	appState(PlayerFourthFinalBlowState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	PlayerFourthFinalBlowState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~PlayerFourthFinalBlowState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class PlayerYakuzaTypeSet : public IYakuzaTypeSet
{
	appState(PlayerYakuzaTypeSet)
public:
	enum PlayerAnimation
	{
		en_punchCross_1_R = YakuzaAnimation::en_num,
		en_punching_1_R,
		en_punching_1_L,
		en_punching_2_R,
		en_punching_3_L,
		en_kick_1,
		en_kick_2,
		num
	};

	PlayerYakuzaTypeSet() : IYakuzaTypeSet(en_campPlayer)
	{
		m_firstAttackID = PlayerFirstAttackState::ID();

		m_firstFinishBrowID = PlayerZeroFinalBlowState::ID();

		m_modelFilePath = "Assets/modelData/Character/Survivalist/Survivalist.tkm";

		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Idle.tka", true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Walking.tka", true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Run.tka", true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Run.tka", true }); // Fighting_Idle 設定予定
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Run.tka", true }); // AimWalk予定
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Run.tka", true }); // AimWalk予定
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Run.tka", true }); // AimWalk予定
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Run.tka", true }); // AimWalk予定
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Dodge_Forward.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Dodge_Right.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Dodge_Left.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Dodge_Back.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Guard.tka", true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Grab_Start.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Grab_Idle_A.tka", true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Grab_Atk_A.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Grab_Throw_A.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Grab_Break_A.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Guard.tka", true });//今のところ掴まれることはないため実装はなし
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Guard.tka", true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Guard.tka", true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Guard.tka", true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/BodyHit.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/BackDeath_P.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/CrossPunch_R_Ev.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Punching_1_R_Ev.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Punching_1_L_Ev.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Punching_2_R_Ev.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Punching_3_L_Ev.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Kick_1_R_EV.tka", false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Survivalist/Animation/Kick_2_L_EV.tka", false });
	}

	void InitStateMachineParam(YakuzaCharacter& useCharacter,YakuzaStateMachine& useStateMachine)
	{

	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{
		AddAttackState<PlayerFirstAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,PlayerSecondAttackState::ID(),PlayerFirstFinalBlowState::ID(),en_punching_1_L,300.0f},
			{ 10.0f,150.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindLigthA });
		AddAttackState<PlayerSecondAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,PlayerThirdAttackState::ID(),PlayerSecondFinalBlowState::ID(),en_punching_1_R,300.0f},
			{ 10.0f,150.0f,SoundId::se_hittingLightB },
			{ SoundId::se_cuttingWindLigthA});
		AddAttackState<PlayerThirdAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,PlayerFourthAttackState::ID(),PlayerThirdFinalBlowState::ID(),en_punching_3_L,300.0f},
			{ 10.0f,150.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindLigthA});
		AddAttackState<PlayerFourthAttackState>(useAttackStateMachine,
			{ 10.0f,150.0f,SoundId::se_hittingLightB },
			{ SoundId::se_cuttingWindLigthA});
		AddAttackState<PlayerZeroFinalBlowState>(
			{ useAttackStateMachine,m_yakuzaCamp,0,0,en_kick_1,150.0f,0.8 },
			{ 5.0f,450.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA });
		AddAttackState<PlayerFirstFinalBlowState>(useAttackStateMachine,
			{ 15.0f,300.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA});
		AddAttackState<PlayerSecondFinalBlowState>(useAttackStateMachine,
			{ 30.0f,300.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA });
		AddAttackState<PlayerThirdFinalBlowState>(useAttackStateMachine,
			{ 30.0f,300.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA });
		AddAttackState<PlayerFourthFinalBlowState>(useAttackStateMachine,
			{ 40.0f,300.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA });
	}
private:
	static TypeSetAutoRegister<PlayerYakuzaTypeSet> typeSet;
};

