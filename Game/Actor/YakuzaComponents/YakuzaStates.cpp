#include "stdafx.h"
#include "YakuzaStates.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\Player\Player.h"

///IdleState

void YakuzaIdleState::OnEnter()
{

}

void YakuzaIdleState::OnUpdate()
{

	m_owner->HasCharactarPlayAnimation(Player::en_idle);

}

void YakuzaIdleState::OnExit()
{

}

///WalkState

void YakuzaWalkState::OnEnter()
{

}

void YakuzaWalkState::OnUpdate()
{

	Vector3 newMoveVec = m_owner->GetMoveVec() * m_owner->GetMoveSpeed();

	//座標を移動
	Vector3 newPos = m_owner->GetHasCharactarCharaCon()->Execute(newMoveVec, g_gameTime->GetFrameDeltaTime());

	//壁に突っ込んだ時に浮かび上がる現象が発生することがあるので、y座標を0にする
	newPos.y = 0.0f;

	//座標を設定
	m_owner->SetHasCharactarPosition(newPos);

	m_owner->GetHasCharactarRot().SetRotationYFromDirectionXZ(m_owner->GetMoveVec());

	m_owner->SetHasCharactarForward(Vector3::Zero);
	m_owner->GetHasCharactarRot().Apply(m_owner->GetHasCharactarForward());

	m_owner->SetMoveVec(Vector3::Zero);

	m_owner->HasCharactarPlayAnimation(Player::en_run);
}

void YakuzaWalkState::OnExit()
{

}

///AttackState

void YakuzaAttackState::OnEnter()
{
	m_owner->SetIsAttack(true);
}

void YakuzaAttackState::OnUpdate()
{

	m_owner->GetAttackStateMachine()->UpdateStateMachine();

}

void YakuzaAttackState::OnExit()
{

}

///FirstAttackComboState
//
//void PlayerFirstAttackComboState::OnEnter()
//{
//
//}
//
//void PlayerFirstAttackComboState::OnUpdate()
//{
//
//	m_owner->HasCharactarPlayAnimation(Player::en_punch_cross);
//
//	if (m_nextComboNum == 0)
//	{
//		if (m_owner->GetAttackFlag())
//		{
//			m_nextComboNum = PlayerSecondAttackComboState::ID();
//		}
//		else if(m_owner->GetFinishBrowFlag())
//		{
//
//		}
//	}
//
//	if (m_owner->GetIsComboTransition())
//	{
//		if (m_nextComboNum != 0)
//		{
//			m_owner->SetNextCombo(m_nextComboNum);
//		}
//	}
//
//	if (!m_owner->IsHasCharactarPlayAnimation())
//	{
//		m_owner->SetIsAttackEnds(false);
//	}
//
//}
//
//void PlayerFirstAttackComboState::OnExit()
//{
//	m_nextComboFlag = false;
//
//	m_nextComboNum = 0;
//
//	m_owner->SetIsComboTransition(false);
//}
//
/////SecondAttackComboState
//
//void PlayerSecondAttackComboState::OnEnter()
//{
//
//}
//
//void PlayerSecondAttackComboState::OnUpdate()
//{
//
//	m_owner->HasCharactarPlayAnimation(Player::en_kick_1);
//
//	if (!m_owner->IsHasCharactarPlayAnimation())
//	{
//		m_owner->SetIsAttackEnds(false);
//	}
//}
//
//void PlayerSecondAttackComboState::OnExit()
//{
//
//}


