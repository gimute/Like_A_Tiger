#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class EnemyAiIdleState;
class EnemyAiTrackingState;

class NormalYakuzaAiWaitingAttackState : public IEnemyAiState
{
	appState(NormalYakuzaAiWaitingAttackState)
public:
	//コンストラクタ
	NormalYakuzaAiWaitingAttackState(YakuzaStateMachine* stateMachine, IEnemyAi* hasEnemyAi)
		: IEnemyAiState(stateMachine, hasEnemyAi)
	{}
	//デストラクタ
	~NormalYakuzaAiWaitingAttackState() = default;

	float m_randomTimer = 0.0f;

	bool m_isMoveLR = false;

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

class NormalYakuzaAi : public IEnemyAi
{
public:
	NormalYakuzaAi(YakuzaStateMachine* controllStateMachine) : IEnemyAi(controllStateMachine)
	{
		AddState<EnemyAiIdleState>(controllStateMachine,this);
		AddState<EnemyAiTrackingState>(controllStateMachine,this);
		AddState<NormalYakuzaAiWaitingAttackState>(controllStateMachine, this);

		InitStateMachineClassName<EnemyAiIdleState>();
	}
	//次のステートを取得
	IStateBase* GetNextState() override;
	//追跡状態に移行できるかどうか
	bool CanChangeTraking();
	//攻撃待機状態に移行できるかどうか
	bool CanChangeWaitingAttack();
private:
	static AiAutoRegister<NormalYakuzaAi> aiSet;
};

