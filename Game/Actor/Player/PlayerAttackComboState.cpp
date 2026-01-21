#include "stdafx.h"
#include "PlayerAttackComboState.h"

#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaAttackAssistSystem.h"

TypeSetAutoRegister<PlayerYakuzaTypeSet> PlayerYakuzaTypeSet::typeSet{ OthersYakuzaType::en_playerYakuza };

namespace 
{
	float ATTACK_MOVE_SPEED = 300.0f;
	float ATTACK_ASSIST_FOV = 0.5f;
	float ATTACK_ASSIST_DIS = 400.0f;
}

void PlayerFourthAttackState::OnEnter()
{
	m_owner->SetIsNextCombo(false);

	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(PlayerFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
}

void PlayerFourthAttackState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	m_owner->GetYakuzaStateMachine()->HasCharactarPlayAnimation(PlayerYakuzaTypeSet::en_punching_2_R, 0.1f);

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

	//攻撃コリジョンが有効だったら移動処理はしない
	if (m_owner->GetIsCreateAttackCollision())
	{
		return;
	}

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	TargetingParam param(ATTACK_ASSIST_DIS, ATTACK_ASSIST_FOV, 0.8f, 0.2f, pos, foward,YakuzaCamp::en_campPlayer);

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

void PlayerFourthAttackState::OnExit()
{

}

//PlayerFirstFinalBrowState

void PlayerFirstFinalBlowState::OnEnter()
{
	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(PlayerFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
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

	//攻撃コリジョンが有効だったら移動処理はしない
	if (m_owner->GetIsCreateAttackCollision())
	{
		return;
	}

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	TargetingParam param(ATTACK_ASSIST_DIS, ATTACK_ASSIST_FOV, 0.8f, 0.2f, pos, foward, YakuzaCamp::en_campPlayer);

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

void PlayerFirstFinalBlowState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}

//PlayerSecondFinalBlowState

void PlayerSecondFinalBlowState::OnEnter()
{
	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(PlayerFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
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

	//攻撃コリジョンが有効だったら移動処理はしない
	if (m_owner->GetIsCreateAttackCollision())
	{
		return;
	}

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	TargetingParam param(ATTACK_ASSIST_DIS, ATTACK_ASSIST_FOV, 0.8f, 0.2f, pos, foward, YakuzaCamp::en_campPlayer);

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

void PlayerSecondFinalBlowState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}

//PlayerThirdFinalBlowState

void PlayerThirdFinalBlowState::OnEnter()
{
	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(PlayerFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
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

	//攻撃コリジョンが有効だったら移動処理はしない
	if (m_owner->GetIsCreateAttackCollision())
	{
		return;
	}

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	TargetingParam param(ATTACK_ASSIST_DIS, ATTACK_ASSIST_FOV, 0.8f, 0.2f, pos, foward, YakuzaCamp::en_campPlayer);

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

void PlayerThirdFinalBlowState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}

//PlayerFourthFinalBlowState

void PlayerFourthFinalBlowState::OnEnter()
{
	YakuzaAttackSEDatas seData = m_owner->GetYakuzaStateMachine()->GetAttackSEDatas(PlayerFirstAttackState::ID());

	SoundManager::Get().PlaySE(seData.m_cuttingWindId);
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

	//攻撃コリジョンが有効だったら移動処理はしない
	if (m_owner->GetIsCreateAttackCollision())
	{
		return;
	}

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	TargetingParam param(ATTACK_ASSIST_DIS, ATTACK_ASSIST_FOV, 0.8f, 0.2f, pos, foward, YakuzaCamp::en_campPlayer);

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

void PlayerFourthFinalBlowState::OnExit()
{
	m_owner->SetIsNextCombo(false);
}