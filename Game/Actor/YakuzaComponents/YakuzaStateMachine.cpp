#include "stdafx.h"
#include "YakuzaStateMachine.h"

#include "YakuzaStates.h"
#include "Actor\Character.h"

IStateBase* YakuzaStateMachine::GetNextState()
{

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

