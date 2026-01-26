#include "stdafx.h"
#include "YakuzaStates.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"
#include "Actor\YakuzaComponents\YakuzaAnimationState.h"
#include "Actor\YakuzaComponents\YakuzaAttackAssistSystem.h"

#include "Sound\SoundManager.h"
#include "Sound\SoundId.h"

///IdleState

void YakuzaIdleState::OnEnter()
{

}

void YakuzaIdleState::OnUpdate()
{

	m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_idle,0.1f);

}

void YakuzaIdleState::OnExit()
{

}

///WalkState

void YakuzaWalkState::OnEnter()
{
	m_owner->SetMoveSpeed(400.0f);
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

	m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_run,0.1f);
}

void YakuzaWalkState::OnExit()
{

}

///AimMoveState

void YakuzaAimMoveState::OnEnter()
{
	m_owner->SetMoveSpeed(100.0f);
}

void YakuzaAimMoveState::OnUpdate()
{
	Vector3 moveVec = m_owner->GetMoveVec();

	//if (moveVec.z == 0.0f &&
	//	moveVec.x == 0.0f)
	//{
	//	moveVec.x = 0.0f;
	//}

	Vector3 newMoveVec = moveVec * m_owner->GetMoveSpeed();

	//座標を移動
	Vector3 newPos = m_owner->GetHasCharactarCharaCon()->Execute(newMoveVec, g_gameTime->GetFrameDeltaTime());

	//壁に突っ込んだ時に浮かび上がる現象が発生することがあるので、y座標を0にする
	newPos.y = 0.0f;

	//座標を設定
	m_owner->SetHasCharactarPosition(newPos);

	Vector3 toTarget = m_owner->GetAimMoveTargetPos() - m_owner->GetHasCharactarPos();

	toTarget.Normalize();

	m_owner->GetHasCharactarRot().SetRotationYFromDirectionXZ(toTarget);

	m_owner->SetHasCharactarForward(Vector3::AxisZ);
	m_owner->GetHasCharactarRot().Apply(m_owner->GetHasCharactarForward());

	Vector3 modelForward = m_owner->GetHasCharactarForward();

	Vector3 modelRight = Cross(Vector3::Up, modelForward);

	float forwardDot = Dot(modelForward, moveVec);

	float rightDot = Dot(modelRight, moveVec);

	AnimationDirection animDir = AnimationDirection::en_forwardDir;;

	if (std::fabs(forwardDot) > std::fabs(rightDot))
	{
		if (forwardDot >= 0)
		{
			m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_aimWalkingFoward,0.1f);
		}
		else
		{
			m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_aimWalkingBack,0.1f);
		}
	}
	else if (std::fabs(forwardDot) < std::fabs(rightDot))
	{
		if (rightDot >= 0)
		{
			m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_aimWalkingLeft,0.1f);
		}
		else
		{
			m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_aimWalkingRigft,0.1f);
		}
	}
	else
	{
		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_fightingIdle,0.1f);
	}
}

void YakuzaAimMoveState::OnExit()
{
	m_owner->SetIsAimMove(false);
}

///AttackState

void YakuzaAttackState::OnEnter()
{
	auto* attackStateMachine = m_owner->GetAttackStateMachine();

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

			attackStateMachine->SetIsCreateAttackCollision(false);

			attackStateMachine->ResetAttackStateMachine();
		}

		return;
	}

}

void YakuzaAttackState::OnExit()
{
	m_owner->SetIsAttack(false);
	m_owner->GetAttackStateMachine()->SetIsAttackEnds(true);
	auto* attackStateMachine = m_owner->GetAttackStateMachine();
	attackStateMachine->ResetAttackStateMachine();
}

//GrabState

void YakuzaGrabState::OnEnter()
{
	//処理前準備
	m_owner->SetIsGrab(true);
	m_owner->SetGrabFlag(false);
	m_owner->SetGrabingToAttackType(-1);

	//近くの敵に向かって行くアシスト処理
	Vector3 foward = m_owner->GetHasCharactarForward();
	Vector3 pos = m_owner->GetHasCharactarPos();
	//ターゲット設定
	TargetingParam param(400.0f, 0.5, 0.8f, 0.2f, pos, foward, m_owner->GetTypeSet().m_yakuzaCamp);
	//ターゲット取得
	YakuzaCharacter* getYakuza = YakuzaAttackAssistSystem::GetIstance()->GetNearYakuza(param);
	//方向決定
	if (getYakuza)
	{
		m_grabMoveVec = getYakuza->GetPosition() - pos;
		m_grabMoveVec.Normalize();
	}
	else
	{
		m_grabMoveVec = foward;
	}

	m_state = en_goGrabMove;
}

void YakuzaGrabState::OnUpdate()
{
	switch (m_state)
	{
	case YakuzaGrabState::en_goGrabMove:
		//位置更新
		MoveProcess();

		//アニメーション再生
		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_grabStart, 0.1f);

		if (m_owner->GetIsGrabing())
		{
			m_owner->HasCharacterGrabingYakuzaThrowPositionAdjustment(
				m_owner->GetHasCharactarForward(),
				m_owner->GetHasCharactarForward() * -1.0f
			);

			m_state = en_grabingMove;

			return;
		}

		if (!m_owner->IsHasCharactarPlayAnimation())
		{
			m_owner->GrabEnd();
		}

		break;
	case YakuzaGrabState::en_grabingMove:
		//位置更新
		m_owner->HasCharacterGrabingProcces();
		//掴んでいるアニメーション再生
		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_grabing, 0.1f);

		//掴み中に通常攻撃の入力があった場合
		if (m_owner->GetAttackFlag() &&
			//現在掴んでいるヤクザがもがき状態である事を確認
			m_owner->GetGrabingToAttackType() == YakuzaAnimation::en_grabed)
		{
			//現在掴んでいるヤクザに攻撃した事を伝える
			m_owner->HasCharacterSendToGrabingOrGrabBedYakuzaData(
				YakuzaAnimation::en_grabAttack
			);

			//自身の掴み攻撃タイプをリセット
			m_owner->SetGrabingToAttackType(-1);

			//掴み攻撃のアニメーションへ移行
			m_state = en_grabingAttackMove;
		}
		//掴み中にフィニッシュブロウの入力があった場合
		else if (m_owner->GetFinishBrowFlag())
		{
			//現在掴んでいるヤクザにフィニッシュブロウをした事を伝える
			m_owner->HasCharacterSendToGrabingOrGrabBedYakuzaData(
				YakuzaAnimation::en_grabThrow
			);

			//投げの位置調整を行う
			m_owner->HasCharacterGrabingYakuzaThrowPositionAdjustment(
				m_owner->GetHasCharactarForward() * -1.0f,
				m_owner->GetHasCharactarForward() * -1.0f
 			);

			//掴み中は攻撃を受け付けないように
			m_owner->HasCharacterSetIsInvincible(true);

			//掴み投げのアニメーションへ移行
			m_state = en_grabingFinshMove;
		}
		//掴み中にもう一度掴みの入力があった場合
		else if (m_owner->GetGrabFlag() ||
			//または掴まれているヤクザが自力での解放を試みた場合
			m_owner->GetGrabingToAttackType() == YakuzaAnimation::en_grabSelfRelease)
		{
			//現在掴んでいるヤクザに掴み解除をした事を伝える
			m_owner->HasCharacterSendToGrabingOrGrabBedYakuzaData(
				YakuzaAnimation::en_grabBeCanceled
			);
			//掴み終了のアニメーションへ移行
			m_state = en_grabingEndProcess;
		}

		break;
	case YakuzaGrabState::en_grabingAttackMove:

		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_grabAttack, 0.1f);

		if (!m_owner->IsHasCharactarPlayAnimation())
		{
			SoundManager::Get().PlaySE(SoundId::se_hittingLightA, false, false, 0.5f);

			m_state = en_grabingMove;
		}

		break;
	case YakuzaGrabState::en_grabingFinshMove:

		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_grabThrow, 0.1f);

		if (!m_owner->IsHasCharactarPlayAnimation())
		{
			m_owner->HasCharacterSetIsInvincible(false);

			m_owner->GrabEnd();
		}

		break;
	case YakuzaGrabState::en_grabingEndProcess:

		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_grabBeCanceled, 0.1f);

		if (!m_owner->IsHasCharactarPlayAnimation())
		{
			m_owner->GrabEnd();
		}

		break;
	}
}

void YakuzaGrabState::MoveProcess()
{
	if (m_owner->IsHasCharacterGrabCollisionActive() || !m_isGoGrabMoveing)
	{
		m_isGoGrabMoveing = false;

		return;
	}

	Vector3 moveVec = m_grabMoveVec * 300.0f;

	Vector3 newPos = m_owner->GetHasCharactarCharaCon()->Execute(moveVec, g_gameTime->GetFrameDeltaTime());

	m_owner->SetHasCharactarPosition(newPos);

	m_owner->GetHasCharactarRot().SetRotationYFromDirectionXZ(moveVec);

	m_owner->SetHasCharactarForward(Vector3::AxisZ);
	m_owner->GetHasCharactarRot().Apply(m_owner->GetHasCharactarForward());

	m_owner->SetMoveVec(Vector3::Zero);
}

void YakuzaGrabState::OnExit()
{
	if (m_owner->GetIsDamage() &&
		m_state == GrabState::en_grabingMove ||
		m_state == GrabState::en_grabingAttackMove)
	{
		m_owner->HasCharacterSendToGrabingOrGrabBedYakuzaData(
			YakuzaAnimation::en_hitBody
		);
	}

	m_owner->GrabEnd();
	m_owner->HasCharacterSetIsInvincible(false);

	m_grabMoveVec = Vector3::Zero;
	m_state = en_grabReady;
	m_isGoGrabMoveing = true;
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

	//効果音を出す
	SoundManager::Get().PlaySE(SoundId::se_kickingGroundA,false,false,0.5f);

	m_owner->SetIsSway(true);
}

void YakuzaSwayState::OnUpdate()
{
	float swayAnimSpeed = m_owner->GetSwayAnimSpeed();

	switch (m_swayDir)
	{
	case YakuzaSwayState::en_forwardDir:
		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_swayForward,0.1f, swayAnimSpeed);
		break;
	case YakuzaSwayState::en_backwardDir:
		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_swayBack, 0.1f, swayAnimSpeed);
		break;
	case YakuzaSwayState::en_rightDir:
		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_swayRight, 0.1f, swayAnimSpeed);
		break;
	case YakuzaSwayState::en_leftDir:
		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_swayLeft, 0.1f, swayAnimSpeed);
		break;
	default:
		break;
	}

	Vector3 moveVec = m_swayVec * m_owner->GetSwaySpeed();

	Vector3 newPos = m_owner->GetHasCharactarCharaCon()->Execute(moveVec, g_gameTime->GetFrameDeltaTime());

	m_owner->SetHasCharactarPosition(newPos);

	if (!m_owner->IsHasCharactarPlayAnimation())
	{
		SoundManager::Get().PlaySE(SoundId::se_GroundFrictionA,false,false,0.5f);

		m_owner->SetIsSway(false);
	}
}

void YakuzaSwayState::OnExit()
{
	m_swayVec = Vector3::Zero;
	m_owner->SetIsSway(false);
	m_owner->SetSwayFlag(false);
}

//DefenseState

void YakuzaDefenseState::OnEnter()
{

}

void YakuzaDefenseState::OnUpdate()
{
	m_owner->SetIsAttack(false);

	KnockBackParam* param = m_owner->GetKnockBackParam();

	m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_defense,0.1f);

	if (m_owner->GetIsDamageKnockBack())
	{
		m_owner->HasCharacterKnockBackProcces(*param);
	}

	if (param->m_isEndKnockBack)
	{
		*param = KnockBackParam{};

		m_owner->SetIsDefense(false);
	}
}

void YakuzaDefenseState::OnExit()
{

}

//DamageState

void YakuzaDamageState::OnEnter()
{

}

void YakuzaDamageState::OnUpdate()
{	
	m_owner->SetIsAttack(false);

	KnockBackParam* param = m_owner->GetKnockBackParam();

	m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_hitBody, 0.1f);

	if (m_owner->GetIsDamageKnockBack())
	{
		m_owner->HasCharacterKnockBackProcces(*param);
	}

	if (!m_owner->IsHasCharactarPlayAnimation() && param->m_isEndKnockBack)
	{
		*param = KnockBackParam{};
		//ノックバック初期化含めてダメージフラグをリセット
		m_owner->SetIsDamage(false,false,*param);
	}
}

void YakuzaDamageState::OnExit()
{

}

//GrabBedState

void YakuzaGrabBedState::OnEnter()
{
	m_state = en_grabBed;
	m_isGrabBedSelfRelease = false;
	m_owner->SetGrabBedToAttackType(-1);
	m_owner->SetGrabBedWeenTime(0.0f);
}

void YakuzaGrabBedState::OnUpdate()
{
	if (m_state == en_grabBed ||
		m_state == en_grabBedAttack ||
		m_state == en_grabBedFinish)
	{
		if (m_owner->GetGrabBedToAttackType() == en_hitBody)
		{
			m_state = en_grabBedBreak;
		}
		else if(m_owner->IsHasCharacterGrabBedEscape(m_owner->GetAttackFlag()))
		{
			m_isGrabBedSelfRelease = true;
		}
	}

	switch (m_state)
	{
	case YakuzaGrabBedState::en_grabBed:

		m_owner->SetAttackFlag(false);
		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_grabed, 0.1f);

		if (m_owner->GetGrabBedToAttackType() == en_grabBeCanceled)
		{
			m_state = en_grabBedBreak;
		}
		else if (m_isGrabBedSelfRelease &&
				!m_owner->GetIsDamage())
		{
			m_owner->HasCharacterSendToGrabingOrGrabBedYakuzaData(en_grabSelfRelease);
		}
		else
		{
			m_owner->HasCharacterSendToGrabingOrGrabBedYakuzaData(en_grabed);
		}

		if (!m_owner->GetIsDamage())
		{
			return;
		}

		if (m_owner->GetGrabBedToAttackType() == en_grabAttack)
		{
			m_state = en_grabBedAttack;
		}
		else if (m_owner->GetGrabBedToAttackType() == en_grabThrow)
		{
			m_state = en_grabBedFinish;
		}

		break;
	case YakuzaGrabBedState::en_grabBedAttack:

		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_grabDamage, 0.1f);

		if (!m_owner->IsHasCharactarPlayAnimation())
		{
			m_owner->HasCharacterSendToGrabingOrGrabBedYakuzaData(en_grabDamage);

			m_owner->HasCharacterGrabBedTakeDamage(en_grabDamage);

			m_owner->SetGrabBedToAttackType(-1);
			m_owner->SetIsDamage(false, false);
			m_state = en_grabBed;
		}		

		break;
	case YakuzaGrabBedState::en_grabBedFinish:

		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_grabThrown, 0.1f);

		if (!m_owner->IsHasCharactarPlayAnimation())
		{
			SoundManager::Get().PlaySE(SoundId::se_hittingHeavyA, false, false, 0.5f);

			m_owner->HasCharacterGrabBedTakeDamage(en_grabThrown);

			m_owner->HasCharacterToGrabBedThrownPositionUpdate();

			m_owner->GrabBedEnd();
		}

		break;
	case YakuzaGrabBedState::en_grabBedBreak:

		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_grabSelfRelease, 0.1f);

		if (!m_owner->IsHasCharactarPlayAnimation())
		{
			m_owner->HasCharacterSendToGrabingOrGrabBedYakuzaData(en_grabSelfRelease);

			m_owner->GrabBedEnd();
		}

		break;
	case YakuzaGrabBedState::en_grabBedSelfRelease:

		m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_grabSelfRelease, 0.1f);

		if (!m_owner->IsHasCharactarPlayAnimation())
		{
			m_owner->GrabBedEnd();
		}

		break;
	}
}

void YakuzaGrabBedState::OnExit()
{
	if (m_owner->IsHasCharacterDead())
	{
		m_owner->SetIsDead(true);
	}

	m_owner->SetGrabBedToAttackType(-1);
	m_owner->SetIsDamage(false, false);
}

//DeadState

void YakuzaDeadState::OnEnter()
{

}

void YakuzaDeadState::OnUpdate()
{
	//攻撃中断
	m_owner->SetIsAttack(false);

	KnockBackParam* param = m_owner->GetKnockBackParam();

	m_owner->HasCharactarPlayAnimation(YakuzaAnimation::en_backDeath, 0.1f);
	
	if (m_owner->GetIsDamageKnockBack())
	{
		m_owner->HasCharacterKnockBackProcces(*param);
	}

	if (!m_owner->IsHasCharactarPlayAnimation())
	{
		if (m_owner->GetIsDamageKnockBack())
		{
			if (!param->m_isEndKnockBack)
			{
				return;
			}
		}

		m_owner->HasCharacterDeadProcces();
	}
}

void YakuzaDeadState::OnExit()
{

}
