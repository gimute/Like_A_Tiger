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
	IEnemyAi* m_owner = nullptr;	
public:
	//コンストラクタ
	IEnemyAiState(YakuzaStateMachine* stateMachine,IEnemyAi* hasEnemyAi) :
		m_hasStateMachine(stateMachine),
		m_owner(hasEnemyAi)
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

