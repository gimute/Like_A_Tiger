#pragma once
#include "Actor\Enemy\EnemyAI\IEnemyAi.h"

#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class NormalYakuzaAi;

class NormalYakuzaAiAttackState : public IEnemyAiState
{
	appState(NormalYakuzaAiAttackState)
protected:
	NormalYakuzaAi* m_owner = nullptr;
public:
	//コンストラクタ
	NormalYakuzaAiAttackState(YakuzaStateMachine* stateMachine, NormalYakuzaAi* hasAi)
		: IEnemyAiState(stateMachine)
		, m_owner(hasAi)
	{}

	//デストラクタ
	~NormalYakuzaAiAttackState() = default;

	//追撃判定
	bool ShouldPerformChaseAttack();
	
	//攻撃範囲内に入っているかどうか
	bool m_isInAttackDis = false;
	//追撃フラグ
	bool m_performAttackFlag = false;
	//攻撃終了フラグ
	bool m_attackEndFlag = true;
	//実行コンボ数
	int m_comboNum = 0;

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

class NormalYakuzaAi : public IEnemyAi
{
public:
	NormalYakuzaAi(YakuzaStateMachine* controllStateMachine) : IEnemyAi(controllStateMachine)
	{
		AddState<EnemyAiIdleState>(controllStateMachine,this);
		AddState<EnemyAiTrackingState>(controllStateMachine,this);
		AddState<EnemyAiWaitingAttackState>(controllStateMachine, this);
		AddState<NormalYakuzaAiAttackState>(controllStateMachine, this);

		InitStateMachineClassName<EnemyAiIdleState>();
	}
	//次のステートを取得
	IStateBase* GetNextState() override;
private:
	//攻撃までの時間
	float m_attackTestTime = 0.0f;
	//攻撃フラグ
	bool m_attackFlag = false;
	//追跡状態に移行できるかどうか
	bool CanChangeTraking();
	//攻撃待機状態に移行できるかどうか
	bool CanChangeWaitingAttack();
	//攻撃に移行できるかどうか
	bool CanChangeAttack();
	//攻撃タイマー
	bool AttackTimer();
public:

	inline void SetAttackFlag(bool setIs) { m_attackFlag = setIs; }

	inline bool GetAttackFlag() { return m_attackFlag; }

	inline void SetAttackTimer(float setTime) { m_attackTestTime = setTime; }

	inline float GetAttackTimer() { return m_attackTestTime; }

private:
	static AiAutoRegister<NormalYakuzaAi> aiSet;
};

