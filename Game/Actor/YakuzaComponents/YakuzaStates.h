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

class YakuzaSwayState : public IStateBase
{
	appState(YakuzaSwayState)
protected:
		YakuzaStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	YakuzaSwayState(YakuzaStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~YakuzaSwayState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
private:
	enum SwayDir
	{
		en_forwardDir,
		en_backwardDir,
		en_rightDir,
		en_leftDir
	};
	//回避方向
	SwayDir m_swayDir = SwayDir::en_forwardDir;
	//回避ベクトル
	Vector3 m_swayVec = Vector3::Zero;
};
