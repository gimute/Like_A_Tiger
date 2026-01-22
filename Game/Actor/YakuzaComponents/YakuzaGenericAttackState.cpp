#include "stdafx.h"
#include "YakuzaGenericAttackState.h"

#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaAttackAssistSystem.h"

namespace
{
	float ATTACK_ASSIST_FOV = 0.5f;
	float ATTACK_ASSIST_DIS = 400.0f;
}

void YakuzaGenericAttackState::OnEnter()
{
	m_owner->SetIsNextCombo(false);

	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(m_hasAttackStateHash);

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
}

void YakuzaGenericAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(m_playAnimationNo, 0.1f,m_attackAnimSpeed);

	if (stateMachine->GetAttackFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = m_nextAttackHash;

		m_owner->SetIsNextCombo(true);
	}
	else if (stateMachine->GetFinishBrowFlag() && !m_owner->GetIsNextCombo())
	{
		m_nextComboHash = m_nextFinalBrowHash;

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

	if (m_owner->GetIsCreateAttackCollision())
	{
		return;
	}

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	TargetingParam param(ATTACK_ASSIST_DIS, ATTACK_ASSIST_FOV, 0.8f, 0.2f, pos, foward,m_yakuzaCamp);

	YakuzaCharacter* targetYakuza = nullptr;

	targetYakuza = YakuzaAttackAssistSystem::GetIstance()->GetNearYakuza(param);

	Vector3 moveVec = Vector3::Zero;

	if (targetYakuza)
	{
		moveVec = targetYakuza->GetPosition() - pos;
		moveVec.Normalize();
		moveVec *= m_attackSpeed;
	}
	else
	{
		moveVec = foward * m_attackSpeed;
	}

	Vector3 newPos = stateMachine->GetHasCharactarCharaCon()->Execute(moveVec, g_gameTime->GetFrameDeltaTime());

	stateMachine->SetHasCharactarPosition(newPos);

	stateMachine->GetHasCharactarRot().SetRotationYFromDirectionXZ(moveVec);

	stateMachine->SetHasCharactarForward(Vector3::AxisZ);
	stateMachine->GetHasCharactarRot().Apply(stateMachine->GetHasCharactarForward());

	stateMachine->SetMoveVec(Vector3::Zero);
}

void YakuzaGenericAttackState::OnExit()
{

}