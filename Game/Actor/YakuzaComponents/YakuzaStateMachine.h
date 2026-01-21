#pragma once
#include "StateMachineComponents\IStateMachine.h"
#include "StateMachineComponents\IState.h"
#include "Actor\YakuzaComponents\YakuzaStates.h"
#include "Actor\YakuzaComponents\YakuzaAttackComboStateMachine.h"
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"

#include "YakuzaCharacter.h"

#include "gameObject\IGameobject.h"

//外部用のノックバック構造体
struct KnockBackParam
{
	//吹き飛ぶ方向
	Vector3 m_direction = Vector3::Zero;
	//吹き飛ぶ力
	float m_power = 0.0f;
	//吹き飛ぶ時間
	float m_duration = 0.0f;

	//ステートマシン内用の変数
	//ノックバックが終了したかどうか
	bool m_isEndKnockBack = false;
	//経過時間
	float m_knockElapsed = 0.0f;


	KnockBackParam() = default;

	KnockBackParam(
		const Vector3& direction,
		float power,
		float duration
	)
		:m_direction(direction),
		m_power(power),
		m_duration(duration)
	{
	}
};

class Character;

class YakuzaCharacter;

class YakuzaStateMachine : public IStateMachine
{
public:
	//コンストラクタ
	YakuzaStateMachine(YakuzaCharacter* charactarPtr) : m_hasCharactar(charactarPtr)
	{
		AddState<YakuzaIdleState>(this);
		AddState<YakuzaWalkState>(this);
		AddState<YakuzaAimMoveState>(this);
		AddState<YakuzaAttackState>(this);
		AddState<YakuzaSwayState>(this);
		AddState<YakuzaDefenseState>(this);
		AddState<YakuzaDamageState>(this);
		AddState<YakuzaDeadState>(this);

		InitStateMachineClassName<YakuzaIdleState>();
	}

	//次のステート値を取得する関数
	IStateBase* GetNextState() override;

private:
	//移動方向
	Vector3 m_moveVec = Vector3::Zero;
	//移動速度 
	float m_moveSpeed = 400.0f;
	//回避速度
	float m_swaySpeed = 600.0f;
	//回避アニメーション速度
	float m_swayAnimSpeed = 3.0f;
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
	//回避中か
	bool m_isSway = false;
	//次のコンボに以降可能かどうか
	bool m_isComboTransition = false;
	//移動方式、狙い移動
	bool m_isAimMove = false;
	//ダメージを受けたかどうか
	bool m_isDamage = false;
	//ダメージを受けてノックバックが発生するかどうか
	bool m_isDamageKnockBack = false;
	//ノックバックのパラメーター
	KnockBackParam m_knockBackParam;
	//死んでしまったかどうか
	bool m_isDead = false;
	//狙い移動のキャラクターの位置
	Vector3 m_aimMoveTargetPos = Vector3::Zero;
	//このステートを扱うCharacterのポインタ
	YakuzaCharacter* m_hasCharactar = nullptr;
	//攻撃専用ステートマシン
	std::unique_ptr<YakuzaAttackComboStateMachine> m_attackStateMachine = nullptr;
	//敵種セット
	std::unique_ptr<IYakuzaTypeSet> m_typeSet;
public:
	///変数系のゲッター＆セッター
	inline void SetMoveVec(const Vector3& vec) { m_moveVec = vec; }

	inline const Vector3& GetMoveVec() { return m_moveVec; }

	inline void SetMoveSpeed(float speed) { m_moveSpeed = speed; }

	inline float GetMoveSpeed() { return m_moveSpeed; }

	inline void SetSwaySpeed(float speed, float animSpeed) { m_swaySpeed = speed; m_swayAnimSpeed = animSpeed; }

	inline float GetSwaySpeed() { return m_swaySpeed; }

	inline float GetSwayAnimSpeed() { return m_swayAnimSpeed; }

	inline void SetAttackFlag(bool setIs) { m_attackFlag = setIs; }

	inline bool GetAttackFlag() { return m_attackFlag; }

	inline void SetFinishBrowFlag(bool setIs) { m_finishBrowFlag = setIs; }

	inline bool GetFinishBrowFlag() { return m_finishBrowFlag; }

	inline void SetSwayFlag(bool setIs) { m_swayFlag = setIs; if (setIs) { m_isSway = true; } }

	inline bool GetSwayFlag() { return m_swayFlag; }

	inline void SetDefenseFlag(bool setIs) { m_defenseFlag = setIs; }

	inline bool GetDefenseFlag() { return m_defenseFlag; }

	inline void SetIsAttack(bool setIs) { m_isAttack = setIs; }

	inline bool GetIsAttack() { return m_isAttack; }

	inline void SetIsSway(bool setIs) { m_isSway = setIs; }

	inline bool GetIsSway() { return m_isSway; }

	inline void SetIsComboTransition(bool setIs) { m_isComboTransition = setIs; }

	inline bool GetIsComboTransition() { return m_isComboTransition; }

	inline void SetIsAimMove(bool setIs) { m_isAimMove = setIs; }

	inline bool GetIsAimMove() { return m_isAimMove; }

	void SetIsDamage(bool setIsDamage, bool setIsKnockBack, KnockBackParam param = KnockBackParam(Vector3{0.0f,0.0f,0.0f},0.0f,0.0f));

	void SetIsDefense(bool setIsKnockBack, KnockBackParam param = KnockBackParam(Vector3{ 0.0f,0.0f,0.0f }, 0.0f, 0.0f));

	void ResetIsKnockBack(const KnockBackParam& param);

	inline bool GetIsDamage() { return m_isDamage; }

	inline bool GetIsDamageKnockBack() { return m_isDamageKnockBack; }

	inline void SetKnockBackParam(const KnockBackParam& param) { m_knockBackParam = param; }

	inline KnockBackParam* GetKnockBackParam() { return &m_knockBackParam; }

	inline void SetIsDead(bool setIs) { m_isDead = setIs; }

	inline bool GetIsDead() { return m_isDead; }
		 
	inline void SetAimMoveTargetPos(const Vector3& setPos) { m_aimMoveTargetPos = setPos; }

	inline const Vector3& GetAimMoveTargetPos() { return m_aimMoveTargetPos; }
	
	inline void SetTypeSet(std::unique_ptr<IYakuzaTypeSet> setType) { m_typeSet = std::move(setType); }

	inline void InitStateMachineParam() { m_typeSet.get()->InitStateMachineParam(*m_hasCharactar,*this); };

	inline IYakuzaTypeSet& GetTypeSet() { return *m_typeSet.get(); }

	inline YakuzaDamageDatas GetTypeSetAttackPower() { return m_typeSet.get()->GetAttackPower(m_attackStateMachine.get()); }

	inline YakuzaAttackSEDatas GetAttackSEDatas(uint32_t stateID) { return m_typeSet.get()->GetAttackSEDatas(stateID); }

	void InitAttackStateMachine(uint32_t firstAttackStateHash,uint32_t firstFinishBrowStateHash);

	void ResetAttackFlagsMachine();

	void SetHasCharactarPosition(const Vector3& pos);

	const Vector3& GetHasCharactarPos();

	void SetPlayerRot(const Quaternion& rot);

	Quaternion& GetHasCharactarRot();

	void SetHasCharactarForward(const Vector3& vec);

	Vector3& GetHasCharactarForward();

	void HasCharactarPlayAnimation(int animationNum, float interpolateTime = 0.0f,float animationSpeed = 1.0f);

	bool IsHasCharactarPlayAnimation();

	bool IsHasCharacterDead();

	bool IsHasCharacterAttackCollisionActive();

	void HasCharacterKnockBackProcces(KnockBackParam& param);

	void HasCharacterDeadProcces();

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	CharacterController* GetHasCharactarCharaCon();

	YakuzaAttackComboStateMachine* GetAttackStateMachine();

	//YakuzaStateMachineが現在なんのステートであるかを取得
	template<typename ClassName>
	inline bool IsGetYakuzaStateMachineNowState()
	{
		return IsNowStateClassName<ClassName>();
	}
private:
	//移動することができるかどうか
	bool CanChangeWalk();
	//格闘攻撃を行えるかどうか
	bool CanChangeAttack();
	//回避行動を行えるかどうか
	bool CanChangeSway();
	//防御行動を行えるかどうか
	bool CanChangeDefense();
	//ダメージを受けたかどうか
	bool CanChangeDamage();
	//死亡したかどうか
	bool CanChangeDead();
};
