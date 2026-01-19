#pragma once
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaGenericAttackState.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class NormalYakuzaFirstAttackState : public YakuzaGenericAttackState
{
	appState(NormalYakuzaFirstAttackState)
public:
	NormalYakuzaFirstAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = NormalYakuzaFirstAttackState::ID();
	}
	~NormalYakuzaFirstAttackState() = default;
};

class NormalYakuzaSecondAttackState : public YakuzaGenericAttackState
{
	appState(NormalYakuzaSecondAttackState)
public:
	NormalYakuzaSecondAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = NormalYakuzaSecondAttackState::ID();
	}

	~NormalYakuzaSecondAttackState() = default;
};

class NormalYakuzaThirdAttackState : public YakuzaGenericAttackState
{
	appState(NormalYakuzaThirdAttackState)
public:
	NormalYakuzaThirdAttackState(
		AttackStateInitData initData
	)
		: YakuzaGenericAttackState(initData)
	{
		m_hasAttackStateHash = NormalYakuzaThirdAttackState::ID();
	}

	~NormalYakuzaThirdAttackState() = default;
};

class NormalYakuzaFourthAttackState : public IStateBase
{
	appState(NormalYakuzaFourthAttackState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	NormalYakuzaFourthAttackState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~NormalYakuzaFourthAttackState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class NormalYakuzaFirstFinalBlowState : public IStateBase
{
	appState(NormalYakuzaFirstFinalBlowState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	NormalYakuzaFirstFinalBlowState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~NormalYakuzaFirstFinalBlowState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class NormalYakuzaSecondFinalBlowState : public IStateBase
{
	appState(NormalYakuzaSecondFinalBlowState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	NormalYakuzaSecondFinalBlowState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~NormalYakuzaSecondFinalBlowState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class NormalYakuzaThirdFinalBlowState : public IStateBase
{
	appState(NormalYakuzaThirdFinalBlowState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	NormalYakuzaThirdFinalBlowState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~NormalYakuzaThirdFinalBlowState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class NormalYakuzaFourthFinalBlowState : public IStateBase
{
	appState(NormalYakuzaFourthFinalBlowState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	NormalYakuzaFourthFinalBlowState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~NormalYakuzaFourthFinalBlowState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

struct NormalYakuzaTypeSet : public IYakuzaTypeSet
{
	appState(NormalYakuzaTypeSet)
public:

	enum NormalYakuzaAnimation
	{
		en_crossPunch_1_R = YakuzaAnimation::en_num,
		en_punching_1_L,
		en_punching_2_R,
		en_punching_3_L
	};

	NormalYakuzaTypeSet() : IYakuzaTypeSet(en_campEnemy)
	{
		m_firstAttackID = NormalYakuzaFirstAttackState::ID();

		m_modelFilePath = "Assets/modelData/Character/Joe/Joe.tkm";

		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Idle.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Walking.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Running.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Fighting_Idle.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/ForwardStep.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/BackStep.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/SideStep_Right.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/SideStep_Left.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Dodge_Right.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Dodge_Right.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Dodge_Left.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Dodge_Left.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Guard.tka",true });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/BodyHit.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/BackDeath_E.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/CrossPunch_R_Ev.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Punching_1_L_Ev_test.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Punching_2_R_Ev.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Punching_3_L_Ev.tka",false });
	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{
		AddAttackState<NormalYakuzaFirstAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,NormalYakuzaSecondAttackState::ID(),NormalYakuzaFirstFinalBlowState::ID(),en_punching_1_L,50.0f},
			{10.0f,150.0f,SoundId::se_hittingLightA},
			{SoundId::se_cuttingWindLigthA});
		AddAttackState<NormalYakuzaSecondAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,NormalYakuzaThirdAttackState::ID(),NormalYakuzaSecondFinalBlowState::ID(),en_punching_3_L,50.0f },
			{ 10.0f,150.0f,SoundId::se_hittingLightB },
			{ SoundId::se_cuttingWindLigthA});
		AddAttackState<NormalYakuzaThirdAttackState>(
			{ useAttackStateMachine,m_yakuzaCamp,NormalYakuzaFourthAttackState::ID(),NormalYakuzaThirdFinalBlowState::ID(),en_punching_1_L,50.0f },
			{ 10.0f,150.0f,SoundId::se_hittingLightA},
			{ SoundId::se_cuttingWindLigthA});
		AddAttackState<NormalYakuzaFourthAttackState>(useAttackStateMachine,
			{ 10.0f,150.0f,SoundId::se_hittingLightB }, 
			{ SoundId::se_cuttingWindLigthA});
		AddAttackState<NormalYakuzaFirstFinalBlowState>(useAttackStateMachine,
			{ 15.0f,300.0f,SoundId::se_hittingLightA },
			{ SoundId::se_cuttingWindHeavyA });
		AddAttackState<NormalYakuzaSecondFinalBlowState>(useAttackStateMachine,
			{ 20.0f,300.0f,SoundId::se_hittingHeavyA },
			{ SoundId::se_cuttingWindHeavyA });
		AddAttackState<NormalYakuzaThirdFinalBlowState>(useAttackStateMachine,
			{ 30.0f,300.0f,SoundId::se_hittingHeavyA }, 
			{ SoundId::se_cuttingWindHeavyA });
		AddAttackState<NormalYakuzaFourthFinalBlowState>(useAttackStateMachine,
			{ 15.0f,300.0f,SoundId::se_hittingHeavyB },
			{ SoundId::se_cuttingWindHeavyA });
	}
private:
	static TypeSetAutoRegister<NormalYakuzaTypeSet> typeSet;
};