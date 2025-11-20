#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAiState.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class EnemyAiIdleState : public IEnemyAiState
{
	appState(EnemyAiIdleState)
public:
	//コンストラクタ
	EnemyAiIdleState(YakuzaStateMachine* stateMachine,IEnemyAi* hasEnemyAi) 
		: IEnemyAiState(stateMachine,hasEnemyAi) 
	{}

	//デストラクタ
	~EnemyAiIdleState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

