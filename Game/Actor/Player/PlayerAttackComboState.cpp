#include "stdafx.h"
#include "PlayerAttackComboState.h"

#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

TypeSetAutoRegister<PlayerYakuzaTypeSet> PlayerYakuzaTypeSet::typeSet{ OthersYakuzaType::en_playerYakuza };

float PlayerYakuzaTypeSet::GetAttackPower(YakuzaAttackComboStateMachine* useAttackStateMachine)
{
	uint32_t nowStateId = useAttackStateMachine->GetNowCombo();

	float attackPower = 0.0f;

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

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(PlayerYakuzaTypeSet::en_punching_1_R, 0.1f);

	if (stateMachine->GetAttackFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = PlayerThirdAttackState::ID();

		m_owner->SetIsNextCombo(true);
	}
	else if (stateMachine->GetFinishBrowFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = PlayerSecondFinalBlowState::ID();

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
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(PlayerYakuzaTypeSet::en_punching_3_L, 0.1f);

	if (stateMachine->GetAttackFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = PlayerFourthAttackState::ID();

		m_owner->SetIsNextCombo(true);
	}
	else if (stateMachine->GetFinishBrowFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = PlayerThirdFinalBlowState::ID();

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

void PlayerThirdAttackState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}

//PlayerFourthAttackState

void PlayerFourthAttackState::OnEnter()
{

}

void PlayerFourthAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(PlayerYakuzaTypeSet::en_punching_1_R, 0.1f);

	if (stateMachine->GetFinishBrowFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = PlayerFourthFinalBlowState::ID();

		m_owner->SetIsNextCombo(true);
	}

	//なんか遷移フラグよりも先にアニメーション終了判定が来てるっぽい

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

void PlayerFourthAttackState::OnExit()
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

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(PlayerYakuzaTypeSet::en_kick_1, 0.1f);

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);

		m_owner->SetIsAttackEnds(true);
	}
}

void PlayerFirstFinalBlowState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}

//PlayerSecondFinalBlowState

void PlayerSecondFinalBlowState::OnEnter()
{

}

void PlayerSecondFinalBlowState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(PlayerYakuzaTypeSet::en_kick_2, 0.1f);

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);

		m_owner->SetIsAttackEnds(true);
	}
}

void PlayerSecondFinalBlowState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}

//PlayerThirdFinalBlowState

void PlayerThirdFinalBlowState::OnEnter()
{

}

void PlayerThirdFinalBlowState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(PlayerYakuzaTypeSet::en_kick_1, 0.1f);

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);

		m_owner->SetIsAttackEnds(true);
	}
}

void PlayerThirdFinalBlowState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}

//PlayerFourthFinalBlowState

void PlayerFourthFinalBlowState::OnEnter()
{

}

void PlayerFourthFinalBlowState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(PlayerYakuzaTypeSet::en_kick_2, 0.1f);

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);

		m_owner->SetIsAttackEnds(true);
	}
}

void PlayerFourthFinalBlowState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}