#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class NormalYakuzaIdleState : public IStateBase
{
	appState(NormalYakuzaIdleState)
protected:
	YakuzaStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	NormalYakuzaIdleState(YakuzaStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~NormalYakuzaIdleState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class NormalYakuzaTrackingState : IStateBase
{
	appState(NormalYakuzaIdleState)
protected:
	YakuzaStateMachine* m_owner = nullptr;
public:
	//コンストラクタ
	NormalYakuzaTrackingState(YakuzaStateMachine* stateMachine) : m_owner(stateMachine) {}
	//デストラクタ
	~NormalYakuzaTrackingState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class NormalYakuzaAi : public IEnemyAi
{
public:
	NormalYakuzaAi(YakuzaStateMachine* controllStateMachine) : IEnemyAi(controllStateMachine) 
	{
		AddState<NormalYakuzaIdleState>(controllStateMachine);

		InitStateMachineClassName<NormalYakuzaIdleState>();
	}
	//次のステートを取得
	IStateBase* GetNextState() override;
	//追跡状態に移行できるかどうか
	bool CanChangeTraking();
private:
	static AiAutoRegister<NormalYakuzaAi> aiSet;
};

