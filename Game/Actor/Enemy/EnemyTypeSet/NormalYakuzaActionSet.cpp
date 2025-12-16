#include "stdafx.h"
#include "NormalYakuzaActionSet.h"
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

TypeSetAutoRegister<NormalYakuzaTypeSet> NormalYakuzaTypeSet::typeSet{ EnemyYakuzaType::en_normalYakuza };

float NormalYakuzaTypeSet::GetAttackPower(YakuzaAttackComboStateMachine* useAttackStateMachine)
{
	uint32_t nowStateId = useAttackStateMachine->GetNowCombo();

	//float attackPower = 0;
	float attackPower = 10;

	if (nowStateId == NormalYakuzaFirstAttackState::ID() || 
		nowStateId == NormalYakuzaSecondAttackState::ID() ||
		nowStateId == NormalYakuzaThirdAttackState::ID() ||
		nowStateId == NormalYakuzaFourthAttackState::ID())
	{
		attackPower = 10.0f;
	}
	else if (nowStateId == NormalYakuzaFirstFinalBlowState::ID())
	{
		attackPower = 15.0f;
	}
	else if (nowStateId == NormalYakuzaSecondFinalBlowState::ID())
	{
		attackPower = 20.0f;
	}
	else if (nowStateId == NormalYakuzaThirdFinalBlowState::ID())
	{
		attackPower = 30.0f;
	}
	else if (nowStateId == NormalYakuzaFourthFinalBlowState::ID())
	{
		attackPower = 15.0f;
	}

	return attackPower;
}

//FirstAttackState

void NormalYakuzaFirstAttackState::OnEnter()
{
	m_owner->SetIsNextCombo(false);
}

void NormalYakuzaFirstAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_punching_1_L,0.1f);

	if (stateMachine->GetAttackFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = NormalYakuzaSecondAttackState::ID();

		m_owner->SetIsNextCombo(true);
	}
	else if (stateMachine->GetFinishBrowFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = NormalYakuzaFirstFinalBlowState::ID();

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

void NormalYakuzaFirstAttackState::OnExit()
{
}

//SecondAttackState

void NormalYakuzaSecondAttackState::OnEnter()
{
	m_owner->SetIsNextCombo(false);
}

void NormalYakuzaSecondAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_punching_2_R, 0.1f);

	if (stateMachine->GetAttackFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = NormalYakuzaThirdAttackState::ID();

		m_owner->SetIsNextCombo(true);
	}
	else if (stateMachine->GetFinishBrowFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = NormalYakuzaSecondFinalBlowState::ID();

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

void NormalYakuzaSecondAttackState::OnExit()
{

}

//ThirdAttackState

void NormalYakuzaThirdAttackState::OnEnter()
{
	m_owner->SetIsNextCombo(false);
}

//最後のコンボはこっちを参考に
void NormalYakuzaThirdAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_punching_3_L, 0.1f);

	if (stateMachine->GetAttackFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = NormalYakuzaFourthAttackState::ID();

		m_owner->SetIsNextCombo(true);
	}
	else if (stateMachine->GetFinishBrowFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = NormalYakuzaThirdFinalBlowState::ID();

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

void NormalYakuzaThirdAttackState::OnExit()
{

}

//FourthAttackState

void NormalYakuzaFourthAttackState::OnEnter()
{
	m_owner->SetIsNextCombo(false);
}

void NormalYakuzaFourthAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_punching_1_L, 0.1f);

	if (stateMachine->GetFinishBrowFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = NormalYakuzaFourthFinalBlowState::ID();

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

void NormalYakuzaFourthAttackState::OnExit()
{

}

//FirstFinalBlowState

void NormalYakuzaFirstFinalBlowState::OnEnter()
{
	m_owner->SetIsNextCombo(false);
}

void NormalYakuzaFirstFinalBlowState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_crossPunch_1_R, 0.1f);

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);

		m_owner->SetIsAttackEnds(true);
	}
}

void NormalYakuzaFirstFinalBlowState::OnExit()
{

}

//SecondFinalBlowState

void NormalYakuzaSecondFinalBlowState::OnEnter()
{
	m_owner->SetIsNextCombo(false);
}

void NormalYakuzaSecondFinalBlowState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_crossPunch_1_R, 0.1f);

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);

		m_owner->SetIsAttackEnds(true);
	}
}

void NormalYakuzaSecondFinalBlowState::OnExit()
{

}

//ThirdFinalBlowState

void NormalYakuzaThirdFinalBlowState::OnEnter()
{
	m_owner->SetIsNextCombo(false);
}

void NormalYakuzaThirdFinalBlowState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_crossPunch_1_R, 0.1f);

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);

		m_owner->SetIsAttackEnds(true);
	}
}

void NormalYakuzaThirdFinalBlowState::OnExit()
{

}

//FourthFinalBlowState

void NormalYakuzaFourthFinalBlowState::OnEnter()
{
	m_owner->SetIsNextCombo(false);
}

void NormalYakuzaFourthFinalBlowState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_crossPunch_1_R, 0.1f);

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);

		m_owner->SetIsAttackEnds(true);
	}
}

void NormalYakuzaFourthFinalBlowState::OnExit()
{

}
