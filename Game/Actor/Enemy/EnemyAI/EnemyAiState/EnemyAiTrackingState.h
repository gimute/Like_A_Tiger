#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAiState.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class EnemyAiTrackingState : public IEnemyAiState
{
	appState(EnemyAiTrackingState)
protected:
	IEnemyAi* m_owner = nullptr;
public:
	//コンストラクタ
	EnemyAiTrackingState(YakuzaStateMachine* stateMachine, IEnemyAi* hasEnemyAi)
		: IEnemyAiState(stateMachine)
		, m_owner(hasEnemyAi)
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
