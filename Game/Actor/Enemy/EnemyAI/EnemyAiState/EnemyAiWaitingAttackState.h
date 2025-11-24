#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAiState.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class EnemyAiWaitingAttackState : public IEnemyAiState
{
	appState(EnemyAiWaitingAttackState)
public:
	//コンストラクタ
	EnemyAiWaitingAttackState(YakuzaStateMachine* stateMachine, IEnemyAi* hasEnemyAi)
		: IEnemyAiState(stateMachine, hasEnemyAi)
	{
	}
	//デストラクタ
	~EnemyAiWaitingAttackState() = default;

	float m_randomTimer = 0.0f;
	//範囲外から出たか
	bool m_isLimitOut = false;
	//範囲外に出た際に前に進むか後ろに進むか
	bool m_LimitOutFB = true;
	//範囲外に出た際にどこまでのラインまで移動するか
	float m_limitOutMoveLine = 0.0f;

	enum WaitingMove
	{
		en_wait,
		en_fowardMove,
		en_backMove,
		en_leftMove,
		en_rightMove,
		num
	};

	int m_waitingMove = WaitingMove::en_wait;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

