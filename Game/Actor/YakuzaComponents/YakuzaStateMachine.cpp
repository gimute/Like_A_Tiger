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

	//回避中なら、回避ボタンが押されたら
	if (CanChangeSway())
	{
		return FindClassNameState<YakuzaSwayState>();
	}

	if (CanChangeDefense())
	{
		return FindClassNameState<YakuzaDefenseState>();
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
	if (GetDefenseFlag() &&
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

