#pragma once
#include "StateMachineComponents\IStateMachine.h"
#include "StateMachineComponents\IState.h"

class YakuzaStateMachine;
class YakuzaAttackComboStateMachine;

class AttackEndState : public IStateBase
{
	appState(AttackEndState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	AttackEndState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~AttackEndState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class ResetState : public IStateBase
{
	appState(ResetState)
protected:
	YakuzaAttackComboStateMachine* m_owner = nullptr;
public:
	ResetState(YakuzaAttackComboStateMachine* hasStateMachine) : m_owner(hasStateMachine) {}

	~ResetState() = default;

	uint32_t m_nextComboHash = 0;

	//ステートイン
	void OnEnter() override;
	//ステートアップデート
	void OnUpdate() override;
	//ステートアウト
	void OnExit() override;
};

class YakuzaAttackComboStateMachine : public IStateMachine
{
public:
	//コンストラクタ
	YakuzaAttackComboStateMachine(YakuzaStateMachine* hasStateMachine) : m_hasStateMachine(hasStateMachine)
	{
		AddState<AttackEndState>(this);
		AddState<ResetState>(this);
	}
	//デストラクタ
	~YakuzaAttackComboStateMachine() = default;

	//次のステート値を取得する関数
	IStateBase* GetNextState() override;

private:
	//この攻撃コンボステートマシンを使用するステートマシン
	YakuzaStateMachine* m_hasStateMachine = nullptr;
	//次のコンボのクラスハッシュ値
	uint32_t m_nextCombo = 0;
	//最初に行う通常攻撃のステートハッシュ値
	uint32_t m_firstAttack = 0;
	//最初に行うフィニッシュブロウのステートのハッシュ値
	uint32_t m_firstFinishBrow = 0;
	//現在のコンボのクラスハッシュ値
	uint32_t m_nowCombo = 0;
	//次のコンボを行うかどうか
	bool m_isNextConbo = false;
	//攻撃中が終了したかどうか
	bool m_isAttackEnds = true;
	//攻撃コリジョンが生成されたかどうか
	bool m_isCriateAttackCollision = false;
public:

	inline void SetNextCombo(uint32_t setComboID) { m_nextCombo = setComboID; }

	inline uint32_t GetNextCombo() { return m_nextCombo; }

	inline void SetIsNextCombo(bool setIs) { m_isNextConbo = setIs; }

	inline bool GetIsNextCombo() { return m_isNextConbo; }

	inline void SetFirstAttack(uint32_t hush) { m_firstAttack = hush; }

	inline uint32_t GetFirstAttack() { return m_firstAttack; }

	inline void SetFirstFinishBrow(uint32_t hush) { m_firstFinishBrow = hush; }

	inline uint32_t GetFirstFinishBrow() { return m_firstFinishBrow; }

	inline void SetNowCombo(uint32_t hush) { m_nowCombo = hush; }

	inline uint32_t GetNowCombo() { return m_nowCombo; }
	 
	inline void SetIsAttackEnds(bool setIs) { m_isAttackEnds = setIs; }

	inline bool GetIsAttackEnds() { return m_isAttackEnds; }

	inline void SetIsCreateAttackCollision(bool setIs) { m_isCriateAttackCollision = setIs; }

	inline bool GetIsCreateAttackCollision() { return m_isCriateAttackCollision; }

	void StartFirstAttack();

	void StartFirstFinishBrow();

	//攻撃終了時に初期化する処理
	void ResetAttackStateMachine();

	YakuzaStateMachine* GetYakuzaStateMachine();
};

