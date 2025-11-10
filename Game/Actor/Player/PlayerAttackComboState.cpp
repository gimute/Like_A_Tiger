#include "stdafx.h"
#include "PlayerAttackComboState.h"

#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\Player\Player.h"

//PlayerFirstAttackState

void PlayerFirstAttackState::OnEnter()
{

}

void PlayerFirstAttackState::OnUpdate()
{

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(Player::en_punch_cross);

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		m_owner->SetIsAttackEnds(true);
	}
}

void PlayerFirstAttackState::OnExit()
{

}