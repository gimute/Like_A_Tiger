#include "stdafx.h"
#include "PlayerAttackComboState.h"

#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

TypeSetAutoRegister<PlayerYakuzaTypeSet> PlayerYakuzaTypeSet::typeSet{ OthersYakuzaType::en_playerYakuza };

float PlayerYakuzaTypeSet::GetAttackPower(YakuzaAttackComboStateMachine* useAttackStateMachine)
{
	uint32_t nowStateId = useAttackStateMachine->GetNowCombo();

	float attackPower;

	if (nowStateId == PlayerFirstAttackState::ID())
	{
		attackPower = 10.0f;
	}
	else if (nowStateId == PlayerSecondAttackState::ID())
	{
		attackPower = 20.0f;
	}
	else if (nowStateId == PlayerThirdAttackState::ID())
	{
		attackPower = 30.0f;
	}
	else if (nowStateId == PlayerFirstFinalBlowState::ID())
	{
		attackPower = 50.0f;
	}

	return attackPower;
}

//PlayerFirstAttackState

void PlayerFirstAttackState::OnEnter()
{

}

//これを見本に
void PlayerFirstAttackState::OnUpdate()
{
	//コンボ判定処理

	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(PlayerYakuzaTypeSet::en_punching_1_L,0.1f);

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
	//コンボ判定処理

	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(PlayerYakuzaTypeSet::en_punching_3_L, 0.1f);

	if (stateMachine->GetAttackFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = PlayerThirdAttackState::ID();

		m_owner->SetIsNextCombo(true);
	}
	else if (stateMachine->GetFinishBrowFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = 0;

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
}

void PlayerSecondAttackState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}

//PlayerThirdAttackState


void PlayerThirdAttackState::OnEnter()
{

}

void PlayerThirdAttackState::OnUpdate()
{
	//コンボ判定処理

	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(PlayerYakuzaTypeSet::en_punching_2_R, 0.1f);

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);

		m_owner->SetIsAttackEnds(true);
	}
}

void PlayerThirdAttackState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}

//PlayerFirstFinalBrowState

void PlayerFirstFinalBlowState::OnEnter()
{

}

void PlayerFirstFinalBlowState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(PlayerYakuzaTypeSet::en_kick_1,0.1f);

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