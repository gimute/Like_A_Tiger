#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\IEnemyAttackAiState.h"

class FastYakuzaAi;

class FastYakuzaAiAttackState : public IEnemyAttackAiState
{
protected:
	FastYakuzaAi* m_owner = nullptr;
public:
	//コンストラクタ
	FastYakuzaAiAttackState(YakuzaStateMachine* stateMachine, FastYakuzaAi* hasAi)
		: IEnemyAttackAiState(stateMachine)
		, m_owner(hasAi)
	{
		m_attackCombos.push_back({ en_normalAttack });
	}

	//デストラクタ
	~FastYakuzaAiAttackState() = default;

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

class FastYakuzaDodgeState : public IEnemyAiState
{
	appState(FastYakuzaDodgeState)
protected:
	IEnemyAi* m_owner = nullptr;
public:
	//コンストラクタ
	FastYakuzaDodgeState(YakuzaStateMachine* stateMachine, IEnemyAi* hasEnemyAi)
		: IEnemyAiState(stateMachine)
		, m_owner(hasEnemyAi)
	{
	}
	//デストラクタ
	~FastYakuzaDodgeState() = default;

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

enum FastYakuzaUniqueState
{
	en_dodgeing = YakuzaAiState::en_YakuzaAIState_UniqueStateNum
};

class FastYakuzaAi : public IEnemyAi
{
public:

	FastYakuzaAi(YakuzaStateMachine* controllStateMachine) : IEnemyAi(controllStateMachine)
	{
		AddState<EnemyAiIdleState>(controllStateMachine, this);
		AddState<EnemyAiTrackingState>(controllStateMachine, this);
		AddState<EnemyAiWaitingAttackState>(controllStateMachine, this);

		InitStateMachineClassName<EnemyAiIdleState>();
	}
	//次のステートを取得
	IStateBase* GetNextState() override;
private:
	//攻撃待機状態に移行できるかどうか
	bool CanChangeWaitingAttack();
	//攻撃に移行できるかどうか
	bool CanChangeAttack();
	//回避できるかどうか
	bool CanChangeDodge();
private:
	static AiAutoRegister<FastYakuzaAi> aiSet;
};

