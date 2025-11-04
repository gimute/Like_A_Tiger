#include "stdafx.h"
#include "PlayerStates.h"
#include "Actor\Player\PlayerStateMachine.h"
#include "Actor\Player\Player.h"

///IdleState

void PlayerIdleState::OnEnter()
{

}

void PlayerIdleState::OnUpdate()
{

	m_owner->PlayerPlayAnimation(Player::en_idle);

}

void PlayerIdleState::OnExit()
{

}

///WalkState

void PlayerWalkState::OnEnter()
{

}

void PlayerWalkState::OnUpdate()
{

	//xyの移動速度を0.0fに設定
	m_owner->SetPlayerMoveVec(Vector3::Zero);

	//左スティックの入力量を取得
	Vector3 stickL;
	stickL.x = m_owner->GetStickAmountLX();
	stickL.y = m_owner->GetStickAmountLY();

	//カメラの前方向と右方向ベクトルを取得
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//y方向には移動しない
	forward.y = 0.0f;
	right.y = 0.0f;

	forward.Normalize();
	right.Normalize();

	right *= stickL.x * 400.0f;
	forward *= stickL.y * 400.0f;

	//移動速度にスティックの入力量を加算
	Vector3 newMoveVec = m_owner->GetPlayerMoveVec() + right + forward;

	m_owner->SetPlayerMoveVec(newMoveVec);

	//座標を移動
	Vector3 newPos = m_owner->GetPlayerCharaCon()->Execute(newMoveVec, g_gameTime->GetFrameDeltaTime());

	//壁に突っ込んだ時に浮かび上がる現象が発生することがあるので、y座標を0にする
	newPos.y = 0.0f;

	//座標を設定
	m_owner->SetPlayerPos(newPos);

	m_owner->PlayerPlayAnimation(Player::en_run);

}

void PlayerWalkState::OnExit()
{

}

///AttackState

void PlayerAttackState::OnEnter()
{
	m_owner->SetIsAttack(true);
}

void PlayerAttackState::OnUpdate()
{

	m_owner->SetNextCombo(PlayerFirstAttackComboState::ID());

}

void PlayerAttackState::OnExit()
{

}

///FirstAttackComboState

void PlayerFirstAttackComboState::OnEnter()
{

}

void PlayerFirstAttackComboState::OnUpdate()
{

	m_owner->PlayerPlayAnimation(Player::en_punch_cross);

	if (m_nextComboNum == 0)
	{
		if (m_owner->GetAttackButtonB())
		{
			m_nextComboNum = PlayerSecondAttackComboState::ID();
		}
		else if(m_owner->GetFinishBrowButtonY())
		{

		}
	}

	if (m_owner->GetIsComboTransition())
	{
		if (m_nextComboNum != 0)
		{
			m_owner->SetNextCombo(m_nextComboNum);
		}
	}

	if (!m_owner->IsPlayerPlayAnimation())
	{
		m_owner->SetIsAttack(false);
	}

}

void PlayerFirstAttackComboState::OnExit()
{
	m_nextComboFlag = false;

	m_nextComboNum = 0;

	m_owner->SetIsComboTransition(false);
}

///SecondAttackComboState

void PlayerSecondAttackComboState::OnEnter()
{

}

void PlayerSecondAttackComboState::OnUpdate()
{

	m_owner->PlayerPlayAnimation(Player::en_kick_1);

	if (!m_owner->IsPlayerPlayAnimation())
	{
		m_owner->SetIsAttack(false);
	}
}

void PlayerSecondAttackComboState::OnExit()
{

}


