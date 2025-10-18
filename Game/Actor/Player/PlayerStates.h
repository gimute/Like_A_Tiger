#pragma once
#include "StateMachineComponents\IStateMachine.h"
#include "StateMachineComponents\IState.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class PlayerStateMachine;

class PlayerIdleState : public IStateBase
{
	appState(PlayerIdleState)
protected:
	PlayerStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	PlayerIdleState(PlayerStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~PlayerIdleState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class PlayerWalkState : public IStateBase
{
	appState(PlayerWalkState)
protected:
	PlayerStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	PlayerWalkState(PlayerStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~PlayerWalkState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};
