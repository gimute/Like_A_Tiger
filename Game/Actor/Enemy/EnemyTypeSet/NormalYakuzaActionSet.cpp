#include "stdafx.h"
#include "NormalYakuzaActionSet.h"
#include "Actor\Enemy\EnemyTypeSet\IEnemyTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

TypeSetAutoRegister<NormalYakuzaTypeSet> NormalYakuzaTypeSet::typeSet{ EnemyType::en_normalYakuza };

float NormalYakuzaTypeSet::GetAttackPower(YakuzaAttackComboStateMachine* useAttackStateMachine)
{
	uint32_t nowStateId = useAttackStateMachine->GetNowCombo();

	float attackPower;

	if (nowStateId == NormalYakuzaFirstAttackState::ID())
	{
		attackPower = 10.0f;
	}
	else if (nowStateId == NormalYakuzaSecondAttackState::ID())
	{
		attackPower = 20.0f;
	}
	else if (nowStateId == NormalYakuzaThirdAttackState::ID())
	{
		attackPower = 30.0f;
	}
	else if (nowStateId == NormalYakuzaFirstFinalBlowState::ID())
	{
		attackPower = 50.0f;
	}

	return attackPower;
}

//FirstAttackState

void NormalYakuzaFirstAttackState::OnEnter()
{

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
	m_owner->SetIsNextCombo(false);
}

//SecondAttackState

void NormalYakuzaSecondAttackState::OnEnter()
{

}

void NormalYakuzaSecondAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_punching_1_L, 0.1f);

	if (stateMachine->GetAttackFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = NormalYakuzaThirdAttackState::ID();

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

void NormalYakuzaSecondAttackState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}

//ThirdAttackState

void NormalYakuzaThirdAttackState::OnEnter()
{

}

//最後のコンボはこっちを参考に
void NormalYakuzaThirdAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_punching_2_R, 0.1f);

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);

		m_owner->SetIsAttackEnds(true);
	}
}

void NormalYakuzaThirdAttackState::OnExit()
{
	m_owner->SetIsNextCombo(false);

	m_owner->SetIsLastCombo(true);
}

//FirstFinalBlowState

void NormalYakuzaFirstFinalBlowState::OnEnter()
{

}

void NormalYakuzaFirstFinalBlowState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_punching_3_L, 0.1f);

	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);
		m_owner->SetIsAttackEnds(true);
	}
}

void NormalYakuzaFirstFinalBlowState::OnExit()
{
	m_owner->SetIsNextCombo(false);

	m_owner->SetIsLastCombo(true);
}