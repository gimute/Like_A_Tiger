#include "stdafx.h"
#include "HirotaYakuzaActionSet.h"

#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaAttackAssistSystem.h"

#include "UI/PoseMenu.h"

TypeSetAutoRegister<HirotaYakuzaActionSet> HirotaYakuzaActionSet::typeSet{ EnemyYakuzaType::en_bossHirotaYakuza };

namespace HirotaYakuzaActionSetConstant
{
	float SP_ATTACK_CHARGE_TIME = 5.0f;
	float ATTACK_ASSIST_FOV = 1.0f;
	float ATTACK_ASSIST_DIS = 1000.0f;
	float RUSH_MOVE_SPEED = 300.0f;
}

//特殊攻撃チャージステート

void HirotaYakuzaSpecialChargeState::OnEnter()
{
	m_owner->SetIsNextCombo(false);

	//チャージ時間を設定
	m_chargeTimer = HirotaYakuzaActionSetConstant::SP_ATTACK_CHARGE_TIME;

	//このステート開始時に攻撃アシストの対象を取得する
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	//視野角を考慮せず、距離優先で攻撃アシストの対象を選定する
	TargetingParam param(
		HirotaYakuzaActionSetConstant::ATTACK_ASSIST_DIS,
		0.0f,
		1.0f,
		0.0f,
		pos,
		foward,
		en_campEnemy,
		false
	);

	m_targetCharacter = YakuzaAttackAssistSystem::GetIstance()->GetNearYakuza(param);
}

void HirotaYakuzaSpecialChargeState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	//チャージアニメーションを再生
	stateMachine->HasCharactarPlayAnimation(HirotaYakuzaActionSet::en_charge_1, 0.1f, 1.0f);

	//次のコンボに遷移可能なら
	if (m_owner->GetIsNextCombo())
	{
		m_owner->SetNextCombo(HirotaYakuzaSpecialAttackRushState::ID());
	}

	m_chargeTimer -= g_gameTime->GetFrameDeltaTime();

	//チャージ時間が0以下になったら遷移可能にする
	if (m_chargeTimer <= 0.0f && !m_owner->GetIsNextCombo())
	{
		m_owner->SetIsNextCombo(true);
	}

	Vector3 m_chargeDirection = Vector3::Zero;

	Vector3 foward = stateMachine->GetHasCharactarForward();
	Vector3 pos = stateMachine->GetHasCharactarPos();

	if (m_targetCharacter)
	{
		m_chargeDirection = m_targetCharacter->GetPosition() - pos;
		m_chargeDirection.Normalize();
		m_chargeDirection *= HirotaYakuzaActionSetConstant::RUSH_MOVE_SPEED;
	}
	else
	{
		m_chargeDirection = foward * HirotaYakuzaActionSetConstant::RUSH_MOVE_SPEED;
	}

	stateMachine->GetHasCharactarRot().SetRotationYFromDirectionXZ(m_chargeDirection);

	stateMachine->SetHasCharactarForward(Vector3::AxisZ);
	stateMachine->GetHasCharactarRot().Apply(stateMachine->GetHasCharactarForward());
}

void HirotaYakuzaSpecialChargeState::OnExit()
{
}

//特殊攻撃ラッシュステート

void HirotaYakuzaSpecialAttackRushState::OnEnter()
{
	//このステート開始時に攻撃アシストの対象を取得する
	auto* stateMachine = m_owner->GetYakuzaStateMachine();
	
	Vector3 foward = stateMachine->GetHasCharactarForward();

	m_rushDirection = foward * HirotaYakuzaActionSetConstant::RUSH_MOVE_SPEED;
}

void HirotaYakuzaSpecialAttackRushState::OnUpdate()
{
	auto* stateMachine = m_owner->GetYakuzaStateMachine();

	//特殊攻撃ラッシュアニメーションを再生
	stateMachine->HasCharactarPlayAnimation(HirotaYakuzaActionSet::en_punchRush_1, 0.1f, 1.0f);

	//特殊攻撃アニメーションが終了したら攻撃終了処理
	if (!m_owner->GetYakuzaStateMachine()->IsHasCharactarPlayAnimation())
	{
		stateMachine->SetIsComboTransition(false);

		m_owner->SetIsAttackEnds(true);
	}

	//攻撃コリジョンが無い場合はこれ以降の処理を行わない
	if (!m_owner->GetIsCreateAttackCollision())
	{
		return;
	}

	Vector3 newPos = stateMachine->GetHasCharactarCharaCon()->Execute(m_rushDirection, g_gameTime->GetFrameDeltaTime());

	stateMachine->SetHasCharactarPosition(newPos);

	stateMachine->GetHasCharactarRot().SetRotationYFromDirectionXZ(m_rushDirection);

	stateMachine->SetHasCharactarForward(Vector3::AxisZ);
	stateMachine->GetHasCharactarRot().Apply(stateMachine->GetHasCharactarForward());

	stateMachine->SetMoveVec(Vector3::Zero);
}

void HirotaYakuzaSpecialAttackRushState::OnExit()
{

}