#include "stdafx.h"
#include "YakuzaStateMachine.h"

#include "YakuzaStates.h"
#include "Actor\Character.h"

IStateBase* YakuzaStateMachine::GetNextState()
{
	if (CanChangeAttack())
	{
		return FindClassNameState<YakuzaAttackState>();
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
		fabsf(m_moveVec.y) >= FLT_EPSILON)
	{
		return true;
	}

	return false;
}

bool YakuzaStateMachine::CanChangeAttack()
{
	if (m_isAttack)
	{
		return true;
	}

	if (m_attackFlag)
	{
		return true;
	}

	if (m_finishBrowFlag)
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

void YakuzaStateMachine::HasCharactarPlayAnimation(int animationNum,float interpolateTime)
{
	m_hasCharactar->GetModelRender()->PlayAnimation(animationNum, interpolateTime);
}

bool YakuzaStateMachine::IsHasCharactarPlayAnimation()
{
	return m_hasCharactar->GetModelRender()->IsPlayingAnimation();
}

void YakuzaStateMachine::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	
	if (wcscmp(eventName, L"CanTransition") == 0)
	{
		m_isComboTransition = true;
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

