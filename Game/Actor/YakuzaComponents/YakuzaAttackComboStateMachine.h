#pragma once
#include "StateMachineComponents\IStateMachine.h"
#include "StateMachineComponents\IState.h"

class YakuzaStateMachine;

class YakuzaAttackComboStateMachine : public IStateMachine
{
public:
	//コンストラクタ
	YakuzaAttackComboStateMachine(YakuzaStateMachine* hasStateMachine) : m_hasStateMachine(hasStateMachine) {}
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
	//次のコンボを行うかどうか
	bool m_isNextConbo = false;
	//攻撃中が終了したかどうか
	bool m_isAttackEnds = false;
	//最初の攻撃を行ったかどうか
	bool m_isUnderFirstAttack = false;
public:

	inline void SetNextCombo(uint32_t setComboID) { m_nextCombo = setComboID; }

	inline uint32_t GetNextCombo() { return m_isNextConbo; }

	inline void SetIsNextCombo(bool setIs) { m_isNextConbo = setIs; }

	inline bool GetIsNextCombo() { return m_isNextConbo; }

	inline void SetFirstAttack(uint32_t hush) { m_firstAttack = hush; }

	inline uint32_t GetFirstAttack() { return m_firstAttack; }

	inline void SetFirstFinishBrow(uint32_t hush) { m_firstFinishBrow = hush; }

	inline uint32_t GetFirstFinishBrow() { return m_firstFinishBrow; }
	
	inline void SetIsAttackEnds(bool setIs) { m_isAttackEnds = setIs; }

	inline bool GetIsAttackEnds() { return m_isAttackEnds; }

	inline void SetIsUnderFirstAttack(bool SetIs) { m_isUnderFirstAttack = SetIs; }

	inline bool GetIsUnderFirstAttack() { return m_isUnderFirstAttack; }

	YakuzaStateMachine* GetYakuzaStateMachine();

};

