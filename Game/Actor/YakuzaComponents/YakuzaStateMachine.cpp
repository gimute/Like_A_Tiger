#include "stdafx.h"
#include "YakuzaStateMachine.h"

#include "YakuzaStates.h"
#include "Actor\Character.h"

IStateBase* YakuzaStateMachine::GetNextState()
{
	//死亡したなら死亡ステートを更新する
	if (CanChangeDead())
	{
		return FindClassNameState<YakuzaDeadState>();
	}

	//ダメージを受けたならダメージステートを更新する
	if (CanChangeDamage())
	{
		return FindClassNameState<YakuzaDamageState>();
	}

	//攻撃中なら現在更新中のアタックステートを更新する
	if (CanChangeAttack())
	{
		return FindClassNameState<YakuzaAttackState>();
	}

	if (CanChangeDefense())
	{
		return FindClassNameState<YakuzaDefenseState>();
	}

	//回避中なら、回避ボタンが押されたら
	if (CanChangeSway())
	{
		return FindClassNameState<YakuzaSwayState>();
	}

	if (GetIsAimMove())
	{
		return FindClassNameState<YakuzaAimMoveState>();
	}

	if (CanChangeWalk())
	{
		return FindClassNameState<YakuzaWalkState>();
	}

	return FindClassNameState<YakuzaIdleState>();
}

bool YakuzaStateMachine::CanChangeWalk()
{
	if (fabsf(m_moveVec.x) >= FLT_EPSILON ||
		fabsf(m_moveVec.z) >= FLT_EPSILON)
	{
		return true;
	}

	return false;
}

bool YakuzaStateMachine::CanChangeAttack()
{
	if (GetFinishBrowFlag() || 
		GetAttackFlag() ||
		GetIsAttack() && 
		!GetIsSway())
	{
		return true;
	}

	return false;
}

bool YakuzaStateMachine::CanChangeSway()
{
	if (GetSwayFlag() ||
		GetIsSway())
	{
		return true;
	}

	return false;
}

bool YakuzaStateMachine::CanChangeDefense()
{
	if (GetIsDamageKnockBack() ||
		GetDefenseFlag() &&
		!GetIsSway() &&
		!GetIsAttack())
	{
		return true;
	}

	return false;
}

bool YakuzaStateMachine::CanChangeDamage()
{
	if (m_isDamage)
	{
		return true;
	}

	return false;
}

bool YakuzaStateMachine::CanChangeDead()
{
	if (m_isDead)
	{
		return true;
	}

	return false;
}

void YakuzaStateMachine::InitAttackStateMachine(uint32_t firstAttackStateHash, uint32_t firstFinishBrowStateHash)
{
	m_attackStateMachine = std::make_unique<YakuzaAttackComboStateMachine>(this);

	m_attackStateMachine->SetFirstAttack(firstAttackStateHash);
	m_attackStateMachine->SetFirstFinishBrow(firstFinishBrowStateHash);
}

void YakuzaStateMachine::ResetAttackFlagsMachine()
{
	m_attackFlag = false;
	m_finishBrowFlag = false;
}

void YakuzaStateMachine::SetIsDefense(bool setIsKnockBack, KnockBackParam param)
{
	//ノックバックフラグ
	m_isDamageKnockBack = setIsKnockBack;
	//ノックバック内パラメーター
	m_knockBackParam = param;
}

void YakuzaStateMachine::SetIsDamage(bool setIsDamage, bool setIsKnockBack, KnockBackParam param)
{
	//ダメージフラグ
	m_isDamage = setIsDamage;
	//ノックバックフラグ
	m_isDamageKnockBack = setIsKnockBack;
	//ノックバック内パラメーター
	m_knockBackParam = param;
}

void YakuzaStateMachine::ResetIsKnockBack(const KnockBackParam& param)
{
	//ノックバックフラグを立て直す
	m_isDamageKnockBack = true;
	//ノックバック内パラメーターを立て直す
	m_knockBackParam = param;

	//アニメーションを一瞬だけ切り替える
	HasCharactarPlayAnimation(YakuzaAnimation::en_fightingIdle, 0.1f);
}

void YakuzaStateMachine::SetHasCharactarPosition(const Vector3& pos) { m_hasCharactar->SetPosition(pos); }

const Vector3& YakuzaStateMachine::GetHasCharactarPos() { return m_hasCharactar->GetPosition(); }

void YakuzaStateMachine::SetPlayerRot(const Quaternion& rot) { m_hasCharactar->SetRotation(rot); }

Quaternion& YakuzaStateMachine::GetHasCharactarRot() { return m_hasCharactar->GetRotation(); }

void YakuzaStateMachine::SetHasCharactarForward(const Vector3& forward) { m_hasCharactar->SetForward(forward); }

Vector3& YakuzaStateMachine::GetHasCharactarForward() { return m_hasCharactar->GetForward(); }

void YakuzaStateMachine::HasCharactarPlayAnimation(int animationNum,float interpolateTime,float animationSpeed)
{
	m_hasCharactar->GetModelRender()->SetAnimationSpeed(animationSpeed);
	m_hasCharactar->GetModelRender()->PlayAnimation(animationNum, interpolateTime);
}

bool YakuzaStateMachine::IsHasCharactarPlayAnimation()
{
	return m_hasCharactar->GetModelRender()->IsPlayingAnimation();
}

bool YakuzaStateMachine::IsHasCharacterDead()
{
	return m_hasCharactar->IsDead();
}

bool YakuzaStateMachine::IsHasCharacterAttackCollisionActive()
{
	return m_hasCharactar->IsAttackCollisionActive();
}

void YakuzaStateMachine::HasCharacterKnockBackProcces(KnockBackParam& param)
{
	//y軸は無視する
	param.m_direction.y = 0.0f;

	//ノックバック方向
	Vector3 knockDir = param.m_direction;
	//ノックバック力
	float knockPower = param.m_power;


	param.m_knockElapsed += g_gameTime->GetFrameDeltaTime();

	float t = param.m_knockElapsed / param.m_duration;
	t = btClamped(t, 0.0f, 1.0f);

	//イージング処理(簡易)
	float ease = 1.0f - t;

	Vector3 moveVec = knockDir * knockPower * ease * g_gameTime->GetFrameDeltaTime();

	Vector3 newPos = GetHasCharactarCharaCon()->Execute(moveVec, 1.0f);

	//座標を設定
	SetHasCharactarPosition(newPos);

	if (t >= 1.0f)
	{
		param.m_isEndKnockBack = true;
	}
}

void YakuzaStateMachine::HasCharacterDeadProcces()
{
	m_hasCharactar->YakuzaCharacterDeadProcces();
}

void YakuzaStateMachine::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"CanTransition") == 0)
	{
		m_isComboTransition = true;
	}
	if (wcscmp(eventName, L"HitBoxOn") == 0)
	{
		m_hasCharactar->SpwanAttackCollision(
			m_hasCharactar,
			20.0f
		);
	}
	if (wcscmp(eventName, L"HitBoxOff") == 0)
	{
		m_hasCharactar->DeleteAttackCollision();
	}
}

CharacterController* YakuzaStateMachine::GetHasCharactarCharaCon()
{
	return m_hasCharactar->GetCharacterController();
}

YakuzaAttackComboStateMachine* YakuzaStateMachine::GetAttackStateMachine()
{
	return m_attackStateMachine.get();
}

