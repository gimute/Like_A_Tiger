#pragma once
#include "StateMachineComponents\IState.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\Enemy\EnemyAI\EnemyAiBlackboard .h"

class IEnemyAi;

class IEnemyAiState : public IStateBase
{
protected:
	YakuzaStateMachine* m_hasStateMachine = nullptr;
	//このステートを持っているステートマシン
public:
	//コンストラクタ
	IEnemyAiState(YakuzaStateMachine* stateMachine) :
		m_hasStateMachine(stateMachine)
	{}

	//デストラクタ
	~IEnemyAiState() = default;

	//ステートイン
	virtual void OnEnter() override;
	//ステートアップデート
	virtual void OnUpdate() override;
	//ステートアウト
	virtual void OnExit() override;
};

