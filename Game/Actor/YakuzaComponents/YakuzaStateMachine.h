#pragma once
#include "StateMachineComponents\IStateMachine.h"
#include "StateMachineComponents\IState.h"
#include "Actor\YakuzaComponents\YakuzaStates.h"
#include "Actor\YakuzaComponents\YakuzaAttackComboStateMachine.h"

class Character;

class YakuzaStateMachine : public IStateMachine
{
public:
	//コンストラクタ
	YakuzaStateMachine(Character* charactarPtr) : m_hasCharactar(charactarPtr)
	{
		AddState<YakuzaIdleState>(this);
		AddState<YakuzaWalkState>(this);
		AddState<YakuzaAttackState>(this);

		ReqestState<YakuzaIdleState>();
	}

	//次のステート値を取得する関数
	IStateBase* GetNextState() override;

private:
	//移動方向
	Vector3 m_moveVec = Vector3::Zero;
	//移動速度 
	float m_moveSpeed = 400.0f;
	//Aボタンが押されたか
	bool m_attackFlag = false;
	//Yボタンが押されたか
	bool m_finishBrowFlag = false;
	//Aボタンが押されたか
	bool m_swayFlag = false;
	//RTまたはLTが押されたか
	bool m_defenseFlag = false;
	//攻撃が中か
	bool m_isAttack = false;
	//次のコンボに以降可能かどうか
	bool m_isComboTransition = false;
	//このステートを扱うCharacterのポインタ
	Character* m_hasCharactar = nullptr;
	//攻撃専用ステートマシン
	std::unique_ptr<YakuzaAttackComboStateMachine> m_attackStateMachine = nullptr;
public:
	///変数系のゲッター＆セッター
	inline void SetMoveVec(const Vector3& vec) { m_moveVec = vec; }

	inline const Vector3& GetMoveVec() { return m_moveVec; }

	inline void SetMoveSpeed(float speed) { m_moveSpeed = speed; }

	inline float GetMoveSpeed() { return m_moveSpeed; }

	inline void SetAttackFlag(bool setIs) { m_attackFlag = setIs; }

	inline bool GetAttackFlag() { return m_attackFlag; }

	inline void SetFinishBrowFlag(bool setIs) { m_finishBrowFlag = setIs; }

	inline bool GetFinishBrowFlag() { return m_finishBrowFlag; }

	inline void SetSwayFlag(bool setIs) { m_swayFlag = setIs; }

	inline bool GetSwayFlag() { return m_swayFlag; }

	inline void SetDefenseFlag(bool setIs) { m_defenseFlag = setIs; }

	inline bool GetDefenseFlag() { return m_defenseFlag; }

	inline void SetIsAttack(bool setIs) { m_isAttack = setIs; }

	inline bool GetIsAttack() { return m_isAttack; }

	inline void SetIsComboTransition(bool setIs) { m_isComboTransition = setIs; }

	inline bool GetIsComboTransition() { return m_isComboTransition; }

	void InitAttackStateMachine(uint32_t firstAttackStateHash,uint32_t firstFinishBrowStateHash);

	void SetHasCharactarPosition(const Vector3& pos);

	const Vector3& GetHasCharactarPos();

	void SetPlayerRot(const Quaternion& rot);

	Quaternion& GetHasCharactarRot();

	void SetHasCharactarForward(const Vector3& vec);

	Vector3& GetHasCharactarForward();

	void HasCharactarPlayAnimation(int animationNum, float interpolateTime = 0.0f);

	bool IsHasCharactarPlayAnimation();

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	CharacterController* GetHasCharactarCharaCon();

	YakuzaAttackComboStateMachine* GetAttackStateMachine();

	///行動可能かを判定する関数
private:
	//移動することができるかどうか
	bool CanChangeWalk();
	//格闘攻撃を行えるかどうか
	bool CanChangeAttack();
};
