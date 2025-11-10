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

	m_owner->SetHasCharactarForward(Vector3::AxisZ);
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
	//アタックステートマシンを取得
	auto* attackStateMachine = m_owner->GetAttackStateMachine();

	//最初の攻撃が始まっていない場合最初の攻撃を行うように設定
	if (attackStateMachine->GetIsAttackEnds())
	{
		if (m_owner->GetAttackFlag())
		{
			attackStateMachine->StartFirstAttack();
		}
		else if (m_owner->GetFinishBrowFlag())
		{
			attackStateMachine->StartFirstFinishBrow();
		}

		return;
	}

	if (!attackStateMachine->GetIsAttackEnds())
	{
		m_owner->GetAttackStateMachine()->UpdateStateMachine();

		if (attackStateMachine->GetIsAttackEnds())
		{
			m_owner->SetIsAttack(false);
		}

		return;
	}
}

void YakuzaAttackState::OnExit()
{

}

//SwayState

void YakuzaSwayState::OnEnter()
{

}

void YakuzaSwayState::OnUpdate()
{

}

void YakuzaSwayState::OnExit()
{

}