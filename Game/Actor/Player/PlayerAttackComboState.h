#pragma once
#include "StateMachineComponents\IState.h"
#include "Actor\YakuzaComponents\YakuzaAttackComboStateMachine.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class PlayerFirstAttackState : public IStateBase
{
	appState(PlayerFirstAttackState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	PlayerFirstAttackState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~PlayerFirstAttackState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class PlayerSecondAttackState : public IStateBase
{
	appState(PlayerSecondAttackState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	PlayerSecondAttackState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~PlayerSecondAttackState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class PlayerThirdAttackState : public IStateBase
{
	appState(PlayerThirdAttackState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	PlayerThirdAttackState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~PlayerThirdAttackState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
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

