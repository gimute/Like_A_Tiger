#include "stdafx.h"
#include "NormalYakuzaActionSet.h"
#include "Actor\YakuzaComponents\IYakuzaTypeSet.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaAttackAssistSystem.h"

TypeSetAutoRegister<NormalYakuzaTypeSet> NormalYakuzaTypeSet::typeSet{ EnemyYakuzaType::en_normalYakuza };

namespace
{
	float ATTACK_MOVE_SPEED = 50.0f;
	float ATTACK_ASSIST_FOV = 0.5f;
	float ATTACK_ASSIST_DIS = 400.0f;
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

	//攻撃コリジョンが有効だったら移動処理はしない
	if (m_owner->GetIsCreateAttackCollision())
	{
		return;
	}

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	TargetingParam param(ATTACK_ASSIST_DIS, ATTACK_ASSIST_FOV, 0.8f, 0.2f, pos, foward, YakuzaCamp::en_campEnemy);

	YakuzaCharacter* targetYakuza = nullptr;
	targetYakuza = YakuzaAttackAssistSystem::GetIstance()->GetNearYakuza(param);

	Vector3 moveVec = Vector3::Zero;

	if (targetYakuza)
	{
		moveVec = targetYakuza->GetPosition() - pos;
		moveVec.Normalize();
		moveVec *= ATTACK_MOVE_SPEED;
	}
	else
	{
		moveVec = foward * ATTACK_MOVE_SPEED;
	}

	Vector3 newPos = stateMachine->GetHasCharactarCharaCon()->Execute(moveVec, g_gameTime->GetFrameDeltaTime());

	stateMachine->SetHasCharactarPosition(newPos);

	stateMachine->GetHasCharactarRot().SetRotationYFromDirectionXZ(moveVec);

	stateMachine->SetHasCharactarForward(Vector3::AxisZ);
	stateMachine->GetHasCharactarRot().Apply(stateMachine->GetHasCharactarForward());

	stateMachine->SetMoveVec(Vector3::Zero);
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

	//攻撃コリジョンが有効だったら移動処理はしない
	if (m_owner->GetIsCreateAttackCollision())
	{
		return;
	}

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	TargetingParam param(ATTACK_ASSIST_DIS, ATTACK_ASSIST_FOV, 0.8f, 0.2f, pos, foward, YakuzaCamp::en_campEnemy);

	YakuzaCharacter* targetYakuza = nullptr;
	targetYakuza = YakuzaAttackAssistSystem::GetIstance()->GetNearYakuza(param);

	Vector3 moveVec = Vector3::Zero;

	if (targetYakuza)
	{
		moveVec = targetYakuza->GetPosition() - pos;
		moveVec.Normalize();
		moveVec *= ATTACK_MOVE_SPEED;
	}
	else
	{
		moveVec = foward * ATTACK_MOVE_SPEED;
	}

	Vector3 newPos = stateMachine->GetHasCharactarCharaCon()->Execute(moveVec, g_gameTime->GetFrameDeltaTime());

	stateMachine->SetHasCharactarPosition(newPos);

	stateMachine->GetHasCharactarRot().SetRotationYFromDirectionXZ(moveVec);

	stateMachine->SetHasCharactarForward(Vector3::AxisZ);
	stateMachine->GetHasCharactarRot().Apply(stateMachine->GetHasCharactarForward());

	stateMachine->SetMoveVec(Vector3::Zero);
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

	//攻撃コリジョンが有効だったら移動処理はしない
	if (m_owner->GetIsCreateAttackCollision())
	{
		return;
	}

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	TargetingParam param(ATTACK_ASSIST_DIS, ATTACK_ASSIST_FOV, 0.8f, 0.2f, pos, foward, YakuzaCamp::en_campEnemy);

	YakuzaCharacter* targetYakuza = nullptr;
	targetYakuza = YakuzaAttackAssistSystem::GetIstance()->GetNearYakuza(param);

	Vector3 moveVec = Vector3::Zero;

	if (targetYakuza)
	{
		moveVec = targetYakuza->GetPosition() - pos;
		moveVec.Normalize();
		moveVec *= ATTACK_MOVE_SPEED;
	}
	else
	{
		moveVec = foward * ATTACK_MOVE_SPEED;
	}

	Vector3 newPos = stateMachine->GetHasCharactarCharaCon()->Execute(moveVec, g_gameTime->GetFrameDeltaTime());

	stateMachine->SetHasCharactarPosition(newPos);

	stateMachine->GetHasCharactarRot().SetRotationYFromDirectionXZ(moveVec);

	stateMachine->SetHasCharactarForward(Vector3::AxisZ);
	stateMachine->GetHasCharactarRot().Apply(stateMachine->GetHasCharactarForward());

	stateMachine->SetMoveVec(Vector3::Zero);
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

	//攻撃コリジョンが有効だったら移動処理はしない
	if (m_owner->GetIsCreateAttackCollision())
	{
		return;
	}

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	TargetingParam param(ATTACK_ASSIST_DIS, ATTACK_ASSIST_FOV, 0.8f, 0.2f, pos, foward, YakuzaCamp::en_campEnemy);

	YakuzaCharacter* targetYakuza = nullptr;
	targetYakuza = YakuzaAttackAssistSystem::GetIstance()->GetNearYakuza(param);

	Vector3 moveVec = Vector3::Zero;

	if (targetYakuza)
	{
		moveVec = targetYakuza->GetPosition() - pos;
		moveVec.Normalize();
		moveVec *= ATTACK_MOVE_SPEED;
	}
	else
	{
		moveVec = foward * ATTACK_MOVE_SPEED;
	}

	Vector3 newPos = stateMachine->GetHasCharactarCharaCon()->Execute(moveVec, g_gameTime->GetFrameDeltaTime());

	stateMachine->SetHasCharactarPosition(newPos);

	stateMachine->GetHasCharactarRot().SetRotationYFromDirectionXZ(moveVec);

	stateMachine->SetHasCharactarForward(Vector3::AxisZ);
	stateMachine->GetHasCharactarRot().Apply(stateMachine->GetHasCharactarForward());

	stateMachine->SetMoveVec(Vector3::Zero);
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

	//攻撃コリジョンが有効だったら移動処理はしない
	if (m_owner->GetIsCreateAttackCollision())
	{
		return;
	}

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	TargetingParam param(ATTACK_ASSIST_DIS, ATTACK_ASSIST_FOV, 0.8f, 0.2f, pos, foward, YakuzaCamp::en_campEnemy);

	YakuzaCharacter* targetYakuza = nullptr;
	targetYakuza = YakuzaAttackAssistSystem::GetIstance()->GetNearYakuza(param);

	Vector3 moveVec = Vector3::Zero;

	if (targetYakuza)
	{
		moveVec = targetYakuza->GetPosition() - pos;
		moveVec.Normalize();
		moveVec *= ATTACK_MOVE_SPEED;
	}
	else
	{
		moveVec = foward * ATTACK_MOVE_SPEED;
	}

	Vector3 newPos = stateMachine->GetHasCharactarCharaCon()->Execute(moveVec, g_gameTime->GetFrameDeltaTime());

	stateMachine->SetHasCharactarPosition(newPos);

	stateMachine->GetHasCharactarRot().SetRotationYFromDirectionXZ(moveVec);

	stateMachine->SetHasCharactarForward(Vector3::AxisZ);
	stateMachine->GetHasCharactarRot().Apply(stateMachine->GetHasCharactarForward());

	stateMachine->SetMoveVec(Vector3::Zero);
}

void NormalYakuzaFourthFinalBlowState::OnExit()
{

}
