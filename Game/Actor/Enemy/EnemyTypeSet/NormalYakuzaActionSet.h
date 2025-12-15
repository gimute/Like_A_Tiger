#pragma once
#include "StateMachineComponents\IState.h"
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class NormalYakuzaFirstAttackState : public IStateBase
{
	appState(NormalYakuzaFirstAttackState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	NormalYakuzaFirstAttackState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~NormalYakuzaFirstAttackState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class NormalYakuzaSecondAttackState : public IStateBase
{
	appState(NormalYakuzaSecondAttackState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	NormalYakuzaSecondAttackState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~NormalYakuzaSecondAttackState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class NormalYakuzaThirdAttackState : public IStateBase
{
	appState(NormalYakuzaThirdAttackState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	NormalYakuzaThirdAttackState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~NormalYakuzaThirdAttackState() = default;

	uint32_t m_nextComboHash = 0;
	
	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
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

	NormalYakuzaTypeSet()
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
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/CrossPunch_R_Ev.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Punching_1_L_Ev_test.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Punching_2_R_Ev.tka",false });
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Punching_3_L_Ev.tka",false });
	}

	void CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) override
	{
		AddAttackState<NormalYakuzaFirstAttackState>(useAttackStateMachine);
		AddAttackState<NormalYakuzaSecondAttackState>(useAttackStateMachine);
		AddAttackState<NormalYakuzaThirdAttackState>(useAttackStateMachine);
		AddAttackState<NormalYakuzaFourthAttackState>(useAttackStateMachine);
		AddAttackState<NormalYakuzaFirstFinalBlowState>(useAttackStateMachine);
		AddAttackState<NormalYakuzaSecondFinalBlowState>(useAttackStateMachine);
		AddAttackState<NormalYakuzaThirdFinalBlowState>(useAttackStateMachine);
		AddAttackState<NormalYakuzaFourthFinalBlowState>(useAttackStateMachine);
	}

	float GetAttackPower(YakuzaAttackComboStateMachine* useAttackStateMachine) override;
private:
	static TypeSetAutoRegister<NormalYakuzaTypeSet> typeSet;
};