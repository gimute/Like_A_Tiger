#include "stdafx.h"
#include "NormalYakuzaActionSet.h"
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

TypeSetAutoRegister<NormalYakuzaTypeSet> NormalYakuzaTypeSet::typeSet{ EnemyYakuzaType::en_normalYakuza };

//FirstAttackState

void NormalYakuzaFirstAttackState::OnEnter()
{
	m_owner->SetIsNextCombo(false);

	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(NormalYakuzaFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
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

	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(NormalYakuzaFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
}

void NormalYakuzaSecondAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_punching_3_L, 0.1f);

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

	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(NormalYakuzaFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
}

//最後のコンボはこっちを参考に
void NormalYakuzaThirdAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_punching_1_L, 0.1f);

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

	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(NormalYakuzaFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
}

void NormalYakuzaFourthAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(NormalYakuzaTypeSet::en_punching_3_L, 0.1f);

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

	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(NormalYakuzaFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
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

	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(NormalYakuzaFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
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

	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(NormalYakuzaFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
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

	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(NormalYakuzaFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
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
