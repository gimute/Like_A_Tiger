#pragma once
#include "StateMachineComponents\IStateMachine.h"
#include "StateMachineComponents\IState.h"
#include "Actor\Player\PlayerStates.h"

class Player;

class PlayerStateMachine : public IStateMachine
{
public:
	//コンストラクタ
	PlayerStateMachine(Player* playerPtr) : m_player(nullptr)
	{
		m_player = playerPtr;

		AddState<PlayerIdleState>(this);
		AddState<PlayerWalkState>(this);
		AddState<PlayerAttackState>(this);
		AddState<PlayerFirstAttackComboState>(this);
		AddState<PlayerSecondAttackComboState>(this);

		ReqestState<PlayerIdleState>();
	}

	//次のステート値を取得する関数
	IStateBase* GetNextState() override;

private:
	//プレイヤーの移動方向
	Vector3 m_playerMoveVec = Vector3::Zero;
	//Lスティックの入力量
	float m_stickAmountLX = 0.0f;
	//Rスティックの入力量
	float m_stickAmountLY = 0.0f;
	//RスティックのX値入力量
	float m_stickAmountRX = 0.0f;
	//RスティックのY値入力量
	float m_stickAmountRY = 0.0f;
	//Rスティックが入力されたか
	bool m_stickR = false;
	//Aボタンが押されたか
	bool m_attackButtonB = false;
	//Yボタンが押されたか
	bool m_finishBrowButtonY = false;
	//Aボタンが押されたか
	bool m_swayButtonA = false;
	//RTまたはLTが押されたか
	bool m_defenseButtonLTandRT = false;
	//攻撃が中か
	bool m_isAttack = false;
	//次の攻撃コンボの攻撃
	uint32_t m_nextCombo = 0;
	//次のコンボに以降可能かどうか
	bool m_isComboTransition = false;
	//プレイヤーのポインタ
	Player* m_player = nullptr;
public:
	///変数系のゲッター＆セッター
	inline void SetPlayerMoveVec(const Vector3& vec) { m_playerMoveVec = vec; }

	inline const Vector3& GetPlayerMoveVec() { return m_playerMoveVec; }

	inline void SetStickAmountLX(float setAmount) { m_stickAmountLX = setAmount; }

	inline float GetStickAmountLX() { return m_stickAmountLX; }

	inline void SetStickAmountLY(float setAmount) { m_stickAmountLY = setAmount; }

	inline float GetStickAmountLY() { return m_stickAmountLY; }

	inline void SetStickAmountRX(float setAmount) { m_stickAmountRX = setAmount; }

	inline float GetStickAmountRX() { return m_stickAmountRX; }

	inline void SetStickAmountRY(float setAmount) { m_stickAmountRY = setAmount; }

	inline float GetStickAmountRY() { return m_stickAmountRY; }

	inline void SetStickR(bool setIs) { m_stickR = setIs; }

	inline bool GetStickR() { return m_stickR; }

	inline void SetAttackButtonB(bool setIs) { m_attackButtonB = setIs; }

	inline bool GetAttackButtonB() { return m_attackButtonB; }

	inline void SetFinishBrowButtonY(bool setIs) { m_finishBrowButtonY = setIs; }

	inline bool GetFinishBrowButtonY() { return m_finishBrowButtonY; }

	inline void SetSwayMoveButtonA(bool setIs) { m_swayButtonA = setIs; }

	inline bool GetSwayMoveButtonA() { return m_swayButtonA; }

	inline void SetDefenseButtonLTandRT(bool setIs) { m_defenseButtonLTandRT = setIs; }

	inline bool GetDefenseButtonLTandRT() { return m_defenseButtonLTandRT; }

	inline void SetIsAttack(bool setIs) { m_isAttack = setIs; }

	inline bool GetIsAttack() { return m_isAttack; }

	inline void SetNextCombo(uint32_t nextCombo) { m_nextCombo = nextCombo; }

	inline uint32_t GetNextCombo() { return m_nextCombo; }

	inline void SetIsComboTransition(bool setIs) { m_isComboTransition = setIs; }

	inline bool GetIsComboTransition() { return m_isComboTransition; }

	void SetPlayerPos(const Vector3& pos);

	const Vector3& GetPlayerPos();

	void SetPlayerRot(const Quaternion& rot);

	const Quaternion& GetPlayerRot();

	void SetPlayerForward(const Vector3& vec);

	const Vector3& GetPlayerForward();

	void PlayerPlayAnimation(int animationNum, float interpolateTime = 0.0f);

	bool IsPlayerPlayAnimation();

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	CharacterController* GetPlayerCharaCon();

	///行動可能かを判定する関数
private:
	//移動することができるかどうか
	bool CanChangeWalk();
	//格闘攻撃を行えるかどうか
	bool CanChangeAttack();
	//コンボ攻撃に移行できるかどうか
	bool CanChangeComboAttack();
};
