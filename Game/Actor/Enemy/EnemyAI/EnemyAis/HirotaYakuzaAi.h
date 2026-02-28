#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\IEnemyAttackAiState.h"

class HirotaYakuzaAi;

class HirotaYakuzaAiAttackState : public IEnemyAttackAiState
{
protected:
	HirotaYakuzaAi* m_owner = nullptr;
public:
	//コンストラクタ
	HirotaYakuzaAiAttackState(YakuzaStateMachine* stateMachine, HirotaYakuzaAi* hasAi)
		: IEnemyAttackAiState(stateMachine)
		, m_owner(hasAi)
	{
		m_attackCombos.push_back({ en_normalAttack,en_normalAttack,en_normalAttack });
		m_attackCombos.push_back({ en_normalAttack,en_finishBrow,en_finishBrow });
	}

	//デストラクタ
	~HirotaYakuzaAiAttackState() = default;

	//攻撃接近処理
	bool ShouldApproachForAttack();
	//攻撃処理
	void PerformAttack();

	//攻撃範囲内に入っているかどうか
	bool m_isInAttackDis = false;
	//攻撃終了フラグ
	bool m_attackEndFlag = true;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class EnemyAiIdleState;
class EnemyAiTrackingState;
class EnemyAiWaitingAttackState;

class HirotaYakuzaAi : public IEnemyAi
{
public:

	HirotaYakuzaAi(YakuzaStateMachine* controllStateMachine) : IEnemyAi(controllStateMachine)
	{
		AddState<EnemyAiIdleState>(controllStateMachine, this);
		AddState<EnemyAiTrackingState>(controllStateMachine, this);
		AddState<EnemyAiWaitingAttackState>(controllStateMachine, this);
		AddState<HirotaYakuzaAiAttackState>(controllStateMachine, this);

		InitStateMachineClassName<EnemyAiIdleState>();
	}
	//次のステートを取得
	IStateBase* GetNextState() override;
private:
	//攻撃に移行できるかどうか
	bool CanChangeAttack();
	//攻撃待機状態に移行できるかどうか
	bool CanChangeWaitingAttack();

	static AiAutoRegister<HirotaYakuzaAi> aiSet;
};

