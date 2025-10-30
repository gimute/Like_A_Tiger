#include "stdafx.h"
#include "PlayerStateMachine.h"

#include "PlayerStates.h"
#include "Player.h"

IStateBase* PlayerStateMachine::GetNextState()
{

	if (CanChangeAttack())
	{

	}

	if (CanChangeWalk())
	{
		return FindClassNameState<PlayerWalkState>();
	}

	return FindClassNameState<PlayerIdleState>();
}

bool PlayerStateMachine::CanChangeWalk()
{
	if (m_stickAmountLX >= 0.001f ||
		m_stickAmountLY >= 0.001f ||
		m_stickAmountLX <= -0.001f||
		m_stickAmountLY <= -0.001f)
	{
		return true;
	}

	return false;
}

bool PlayerStateMachine::CanChangeAttack()
{
	if (m_attackButtonB)
	{
		return true;
	}

	if (m_finishBrowButtonY)
	{
		return true;
	}

	return false;
}

void PlayerStateMachine::SetPlayerPos(const Vector3& pos) { m_player->SetPosition(pos); }

const Vector3& PlayerStateMachine::GetPlayerPos() { return m_player->GetPosition(); }

void PlayerStateMachine::SetPlayerRot(const Quaternion& rot) { m_player->SetRotation(rot); }

const Quaternion& PlayerStateMachine::GetPlayerRot() { return m_player->GetRotation(); }

void PlayerStateMachine::SetPlayerForward(const Vector3& forward) { m_player->SetPlayerForward(forward); }

const Vector3& PlayerStateMachine::GetPlayerForward() { return m_player->GetPlayerForward(); }

void PlayerStateMachine::PlayerPlayAnimation(int animationNum,float interpolateTime)
{
	m_player->GetModelRender()->PlayAnimation(animationNum, interpolateTime);
}

CharacterController* PlayerStateMachine::GetPlayerCharaCon()
{
	return m_player->GetCharacterController();
}

