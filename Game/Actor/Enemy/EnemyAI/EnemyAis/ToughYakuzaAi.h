#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"
#include "Actor\Enemy\EnemyAI\EnemyAiState\IEnemyAttackAiState.h"

class ToughYakuzaAi;

class ToughYakuzaAiAttackState : public IEnemyAttackAiState
{
protected:
	ToughYakuzaAi* m_owner = nullptr;
public:
	//コンストラクタ
	ToughYakuzaAiAttackState(YakuzaStateMachine* stateMachine, ToughYakuzaAi* hasAi)
		: IEnemyAttackAiState(stateMachine)
		, m_owner(hasAi)
	{
		m_attackCombos.push_back({ en_finishBrow });
	}

	//デストラクタ
	~ToughYakuzaAiAttackState() = default;

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

enum ToughYakuzaUniqueState
{
	en_guard = YakuzaAiState::en_YakuzaAIState_UniqueStateNum
};

class ToughYakuzaAi : public IEnemyAi
{
public:

	ToughYakuzaAi(YakuzaStateMachine* controllStateMachine) : IEnemyAi(controllStateMachine)
	{
		AddState<EnemyAiIdleState>(controllStateMachine, this);
		AddState<EnemyAiTrackingState>(controllStateMachine, this);
		AddState<EnemyAiWaitingAttackState>(controllStateMachine, this);
		AddState<ToughYakuzaAiAttackState>(controllStateMachine, this);
	
		InitStateMachineClassName<EnemyAiIdleState>();
	}
	//次のステートを取得
	IStateBase* GetNextState() override;

	inline void SetGaurdCoolTime(float time)
	{
		m_guardCoolTime = time;
	}
private:
	float m_guardCoolTime = 0.0f;
	//攻撃待機状態に移行できるかどうか
	bool CanChangeWaitingAttack();
	//攻撃に移行できるかどうか
	bool CanChangeAttack();
	//回避できるかどうか
	bool CanChangeGuard();
private:
	static AiAutoRegister<ToughYakuzaAi> aiSet;
};

