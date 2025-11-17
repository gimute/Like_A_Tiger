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

	NormalYakuzaTypeSet()
	{
		SetFirstAttackID(NormalYakuzaFirstAttackState::ID());
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