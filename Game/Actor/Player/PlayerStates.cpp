#include "stdafx.h"
#include "PlayerStates.h"
#include "Actor\Player\PlayerStateMachine.h"
#include "Actor\Player\Player.h"

///IdleState

void PlayerIdleState::OnEnter()
{

}

void PlayerIdleState::OnUpdate()
{

	m_owner->PlayerPlayAnimation(Player::en_idle);

}

void PlayerIdleState::OnExit()
{

}

///WalkState

void PlayerWalkState::OnEnter()
{

}

void PlayerWalkState::OnUpdate()
{

	m_owner->PlayerPlayAnimation(Player::en_run);

}

void PlayerWalkState::OnExit()
{

}
