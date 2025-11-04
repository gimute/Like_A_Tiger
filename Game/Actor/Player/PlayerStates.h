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

class PlayerAttackState : public IStateBase
{
	appState(PlayerAttackState)
protected:
	PlayerStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	PlayerAttackState(PlayerStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~PlayerAttackState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class PlayerFirstAttackComboState : public IStateBase
{
	appState(PlayerFirstAttackComboState)
protected:
	PlayerStateMachine* m_owner = nullptr;
private:
	bool m_nextComboFlag = false;
	uint32_t m_nextComboNum = 0;
public:
	//コンストラクタ
	PlayerFirstAttackComboState(PlayerStateMachine* stateMachine) : m_owner(stateMachine) {};
	//デストラクタ
	~PlayerFirstAttackComboState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class PlayerSecondAttackComboState : public IStateBase
{
	appState(PlayerSecondAttackComboState)
protected:
	PlayerStateMachine* m_owner = nullptr;
private:
	bool m_nextComboFlag = false;
	uint32_t m_nextComboNum = 0;
public:
	//コンストラクタ
	PlayerSecondAttackComboState(PlayerStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~PlayerSecondAttackComboState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};