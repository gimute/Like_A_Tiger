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

	m_owner->HasCharactarPlayAnimation(Player::en_idle,0.1f);

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

	m_owner->HasCharactarPlayAnimation(Player::en_run,0.1f);
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
	m_owner->SetIsAttack(false);
	m_owner->GetAttackStateMachine()->SetIsAttackEnds(true);
}

//SwayState

void YakuzaSwayState::OnEnter()
{
	//回避入力時の入力方向を取得
	m_swayVec = m_owner->GetMoveVec();
	
	if (m_swayVec.x == 0.0f &&
		m_swayVec.z == 0.0f)
	{
		m_swayVec = m_owner->GetHasCharactarForward() * -1.0f;
	}

	//正規化
	m_swayVec.Normalize();

	Vector3 modelForward = m_owner->GetHasCharactarForward();

	Vector3 modelRight = Cross(Vector3::Up, modelForward);

	float forwardDot = Dot(modelForward, m_swayVec);

	float rightDot = Dot(modelRight, m_swayVec);

	if (std::fabs(forwardDot) > std::fabs(rightDot))
	{
		if (forwardDot >= 0)
		{
			m_swayDir = SwayDir::en_forwardDir;
		}
		else
		{
			m_swayDir = SwayDir::en_backwardDir;
		}
	}
	else
	{
		if (rightDot >= 0)
		{
			m_swayDir = SwayDir::en_rightDir;
		}
		else
		{
			m_swayDir = SwayDir::en_leftDir;
		}
	}

	m_owner->SetIsSway(true);
}

void YakuzaSwayState::OnUpdate()
{
	switch (m_swayDir)
	{
	case YakuzaSwayState::en_forwardDir:
		m_owner->HasCharactarPlayAnimation(Player::en_swayForward,0.1f,3.0f);
		break;
	case YakuzaSwayState::en_backwardDir:
		m_owner->HasCharactarPlayAnimation(Player::en_swayBack, 0.1f, 3.0f);
		break;
	case YakuzaSwayState::en_rightDir:
		m_owner->HasCharactarPlayAnimation(Player::en_swayRight, 0.1f, 3.0f);
		break;
	case YakuzaSwayState::en_leftDir:
		m_owner->HasCharactarPlayAnimation(Player::en_swayLeft, 0.1f, 3.0f);
		break;
	default:
		break;
	}

	Vector3 moveVec = m_swayVec * m_owner->GetSwaySpeed();

	Vector3 newPos = m_owner->GetHasCharactarCharaCon()->Execute(moveVec, g_gameTime->GetFrameDeltaTime());

	m_owner->SetHasCharactarPosition(newPos);

	if (!m_owner->IsHasCharactarPlayAnimation())
	{
		m_owner->SetIsSway(false);
	}
}

void YakuzaSwayState::OnExit()
{
	m_swayVec = Vector3::Zero;
	m_owner->SetIsSway(false);
}

//DefenseState

void YakuzaDefenseState::OnEnter()
{

}

void YakuzaDefenseState::OnUpdate()
{
	m_owner->HasCharactarPlayAnimation(Player::en_defense,0.1f);
}

void YakuzaDefenseState::OnExit()
{

}