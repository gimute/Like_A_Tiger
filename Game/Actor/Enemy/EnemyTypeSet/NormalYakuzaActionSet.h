#pragma once
#include "StateMachineComponents\IState.h"
#include "Actor\Enemy\EnemyTypeSet\IEnemyTypeSet.h"
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

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

struct NormalYakuzaTypeSet : public IEnemyTypeSet
{
	appState(NormalYakuzaTypeSet)
public:

	enum NormalYakuzaAnimation
	{
		en_punch = YakuzaAnimation::en_num
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
		m_animationDataList.push_back({ "Assets/modelData/Character/Joe/Animation/Punching.tka",true });
	}

	std::unordered_map<uint32_t, std::unique_ptr<IStateBase>> CreateActions(YakuzaAttackComboStateMachine* useAttackStateMachine) const override
	{
		std::unordered_map<uint32_t,std::unique_ptr<IStateBase>> actions;

		actions.emplace(NormalYakuzaFirstAttackState::ID(), std::make_unique<NormalYakuzaFirstAttackState>(useAttackStateMachine));

		return actions;
	}
private:
	static TypeSetAutoRegister<NormalYakuzaTypeSet> typeSet;

	
};