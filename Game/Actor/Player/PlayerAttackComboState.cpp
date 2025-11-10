#include "stdafx.h"
#include "PlayerAttackComboState.h"

#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\Player\Player.h"

//PlayerFirstAttackState

void PlayerFirstAttackState::OnEnter()
{

}

//これを見本に
void PlayerFirstAttackState::OnUpdate()
{
	//コンボ判定処理

	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(Player::en_punch_cross);

	if (stateMachine->GetAttackFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = PlayerSecondAttackState::ID();

		m_owner->SetIsNextCombo(true);
	}
	else if (stateMachine->GetFinishBrowFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = PlayerFirstFinalBlowState::ID();

		m_owner->SetIsNextCombo(true);
	}

	if (stateMachine->GetIsComboTransition() && m_owner->GetIsNextCombo())
	{
		m_owner->SetNextCombo(m_nextComboHash);

		stateMachine->SetIsComboTransition(false);
	}

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);

		m_owner->SetIsAttackEnds(true);
	}

	//移動処理、必要あれば
	//Vector3 moveVec = stateMachine->GetHasCharactarForward() * 40.0f;

	//Vector3 newPos = stateMachine->GetHasCharactarCharaCon()->Execute(moveVec, g_gameTime->GetFrameDeltaTime());

	//stateMachine->SetHasCharactarPosition(newPos);

}

void PlayerFirstAttackState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}

//PlayerSecondAttackState

void PlayerSecondAttackState::OnEnter()
{

}

void PlayerSecondAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(Player::en_kick_1);

	if (stateMachine->GetIsComboTransition())
	{
		if (stateMachine->GetAttackFlag())
		{

		}
		else if (stateMachine->GetFinishBrowFlag())
		{

		}
	}

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		m_owner->SetIsAttackEnds(true);
	}

}

void PlayerSecondAttackState::OnExit()
{

}

//PlayerFirstFinalBrowState

void PlayerFirstFinalBlowState::OnEnter()
{

}

void PlayerFirstFinalBlowState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(Player::en_punch_1);

	if (stateMachine->GetIsComboTransition())
	{
		if (stateMachine->GetAttackFlag())
		{

		}
		else if (stateMachine->GetFinishBrowFlag())
		{

		}
	}

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		m_owner->SetIsAttackEnds(true);
	}

}

void PlayerFirstFinalBlowState::OnExit()
{

}