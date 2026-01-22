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

class YakuzaAimMoveState : public IStateBase
{
	appState(YakuzaAimMoveState)
protected:
	YakuzaStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	YakuzaAimMoveState(YakuzaStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~YakuzaAimMoveState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
private:
	enum AnimationDirection
	{
		en_forwardDir,
		en_backwardDir,
		en_rightDir,
		en_leftDir
	};
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

class YakuzaGrabState : public IStateBase
{
	appState(YakuzaGrabState)
protected:
	YakuzaStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	YakuzaGrabState(YakuzaStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~YakuzaGrabState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
private:
	void MoveProcess();

	enum GrabState
	{
		//処理前
		en_grabReady,
		//掴みに行く動き
		en_goGrabMove,
		//掴み中の動き
		en_grabingMove
	};

	GrabState m_state = en_grabReady;

	Vector3 m_grabMoveVec = Vector3::Zero;

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


class YakuzaDefenseState : public IStateBase
{
	appState(YakuzaDefenseState)
protected:
	YakuzaStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	YakuzaDefenseState(YakuzaStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~YakuzaDefenseState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class YakuzaDamageState : public IStateBase
{
	appState(YakuzaDamageState)
protected:
	YakuzaStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	YakuzaDamageState(YakuzaStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~YakuzaDamageState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class YakuzaDeadState : public IStateBase
{
	appState(YakuzaDeadState)
protected:
	YakuzaStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	YakuzaDeadState(YakuzaStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~YakuzaDeadState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};