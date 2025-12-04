#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAiState.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class IEnemyAttackAiState : public IEnemyAiState
{
	appState(IEnemyAttackAiState)
public:
	IEnemyAttackAiState(YakuzaStateMachine* hasStateMachine)
		: IEnemyAiState(hasStateMachine)
	{}

	~IEnemyAttackAiState() = default;

	//ステートイン
	virtual void OnEnter() override;
	//ステートアップデート
	virtual void OnUpdate() override;
	//ステートアウト
	virtual void OnExit() override;
};

