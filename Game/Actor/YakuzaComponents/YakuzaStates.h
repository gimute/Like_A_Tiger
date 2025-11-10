#pragma once
#include "StateMachineComponents\IStateMachine.h"
#include "StateMachineComponents\IState.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class YakuzaStateMachine;

class YakuzaIdleState : public IStateBase
{
	appState(YakuzaIdleState)
protected:
	YakuzaStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	YakuzaIdleState(YakuzaStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~YakuzaIdleState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class YakuzaWalkState : public IStateBase
{
	appState(YakuzaWalkState)
protected:
	YakuzaStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	YakuzaWalkState(YakuzaStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~YakuzaWalkState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class YakuzaAttackState : public IStateBase
{
	appState(YakuzaAttackState)
protected:
	YakuzaStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	YakuzaAttackState(YakuzaStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~YakuzaAttackState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};
