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

		ReqestState<PlayerIdleState>();
	}

	//次のステート値を取得する関数
	IStateBase* GetNextState() override;

private:
	//プレイヤーの移動方向
	Vector3 m_playerMoveVec = Vector3::Zero;
	//Lスティックの入力量
	float m_stickAmount = 0.0f;
	//Aボタンが押されたか
	bool m_attackButtonB = false;

	bool m_finishBrowButtonY = false;

	bool m_swayButtonA = false;

	bool m_defenseButtonLTandRT = false;

	Player* m_player = nullptr;
public:
	///変数系のゲッター＆セッター

	inline void SetPlayerMoveVec(const Vector3& vec) { m_playerMoveVec = vec; }

	inline const Vector3& GetPlayerMoveVec() { return m_playerMoveVec; }

	inline void SetStickAmount(float setAmount) { m_stickAmount = setAmount; }

	inline float GetStickAmount() { return m_stickAmount; }

	inline void SetAttackButtonB(bool setIs) { m_attackButtonB = setIs; }

	inline bool GetAttackButtonB() { return m_attackButtonB; }

	inline void SetFinishBrowButtonY(bool setIs) { m_finishBrowButtonY = setIs; }

	inline bool GetFinishBrowButtonY() { return m_finishBrowButtonY; }

	inline void SetSwayMoveButtonA(bool setIs) { m_swayButtonA = setIs; }

	inline bool GetSwayMoveButtonA() { return m_swayButtonA; }

	inline void SetDefenseButtonLTandRT(bool setIs) { m_defenseButtonLTandRT = setIs; }

	inline bool GetDefenseButtonLTandRT() { return m_defenseButtonLTandRT; }

	void PlayerPlayAnimation(int animationNum, float interpolateTime = 0.0f);

	///行動可能かを判定する関数
private:
	//移動することができるかどうか
	bool CanChangeWalk();
	//格闘攻撃を行えるかどうか
	bool CanChangeAttack();
};
