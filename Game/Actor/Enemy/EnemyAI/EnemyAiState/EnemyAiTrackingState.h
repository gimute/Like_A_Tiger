#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAiState.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class EnemyAiTrackingState : public IEnemyAiState
{
	appState(EnemyAiTrackingState)
public:
	//コンストラクタ
	EnemyAiTrackingState(YakuzaStateMachine* stateMachine, IEnemyAi* hasEnemyAi)
		: IEnemyAiState(stateMachine, hasEnemyAi)
	{}

	//デストラクタ
	~EnemyAiTrackingState() = default;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};
