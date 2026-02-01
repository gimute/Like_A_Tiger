#include "stdafx.h"
#include "YakuzaStateMachine.h"

#include "YakuzaStates.h"
#include "Actor\Character.h"

#include "Actor\YakuzaComponents\YakuzaCharacterDamageManager.h"

#include "Sound\SoundManager.h"
#include "Sound\SoundId.h"

IStateBase* YakuzaStateMachine::GetNextState()
{
	if (CanChangeGrabBed())
	{
		return FindClassNameState<YakuzaGrabBedState>();
	}

	//死亡したなら死亡ステートを更新する
	if (CanChangeDead())
	{
		return FindClassNameState<YakuzaDeadState>();
	}

	//ダメージを受けたならダメージステートを更新する
	if (CanChangeDamage())
	{
		return FindClassNameState<YakuzaDamageState>();
	}

	if (CanChangeGrab())
	{
		return FindClassNameState<YakuzaGrabState>();
	}

	//攻撃中なら現在更新中のアタックステートを更新する
	if (CanChangeAttack())
	{
		return FindClassNameState<YakuzaAttackState>();
	}

	if (CanChangeDefense())
	{
		return FindClassNameState<YakuzaDefenseState>();
	}

	//回避中なら、回避ボタンが押されたら
	if (CanChangeSway())
	{
		return FindClassNameState<YakuzaSwayState>();
	}

	if (GetIsAimMove())
	{
		return FindClassNameState<YakuzaAimMoveState>();
	}

	if (CanChangeWalk())
	{
		return FindClassNameState<YakuzaWalkState>();
	}

	return FindClassNameState<YakuzaIdleState>();
}

bool YakuzaStateMachine::CanChangeWalk()
{
	if (fabsf(m_moveVec.x) >= FLT_EPSILON ||
		fabsf(m_moveVec.z) >= FLT_EPSILON)
	{
		return true;
	}

	return false;
}

bool YakuzaStateMachine::CanChangeAttack()
{
	if (m_isAttack)
	{
		return true;
	}

	if ((m_attackFlag || m_finishBrowFlag) 
		&& !m_isSway
		&& !m_defenseFlag)
	{
		return true;
	}

	return false;
}

bool YakuzaStateMachine::CanChangeSway()
{
	if (m_isSway)
	{
		return true;
	}

	if (m_swayFlag &&
		!m_isGrab && 
		!m_isGrabing
	)
	{
		return true;
	}

	return false;
}

bool YakuzaStateMachine::CanChangeDefense()
{
	if (m_isDamageKnockBack)
	{
		return true;
	}

	if ((m_defenseFlag) &&
		!m_isSway &&
		!m_isAttack)
	{
		return true;
	}

	return false;
}

bool YakuzaStateMachine::CanChangeDamage()
{
	if (m_isDamage)
	{
		return true;
	}

	return false;
}

bool YakuzaStateMachine::CanChangeDead()
{
	if (m_isDead)
	{
		return true;
	}

	return false;
}

Bone* YakuzaStateMachine::GetCharacterBone(const wchar_t* boneName)
{
	return m_hasCharactar->GetBone(boneName);
}

bool YakuzaStateMachine::CanChangeGrab()
{
	//掴み継続は掴まれておらずダメージを受けていない場合
	if (m_isGrab && 
		!m_isGrabbed && !m_isDamage)
	{
		return true;
	}

	//掴み中開始は回避していない場合かつ防御していないかつ攻撃していない場合
	if (m_grabFlag &&
		!m_isSway && !m_defenseFlag && !m_isAttack)
	{
		return true;
	}

	return false;
}

bool YakuzaStateMachine::CanChangeGrabBed()
{
	if (m_isGrabbed)
	{
		return true;
	}

	return false;
}

void YakuzaStateMachine::InitAttackStateMachine(uint32_t firstAttackStateHash, uint32_t firstFinishBrowStateHash)
{
	m_attackStateMachine = std::make_unique<YakuzaAttackComboStateMachine>(this);

	m_attackStateMachine->SetFirstAttack(firstAttackStateHash);
	m_attackStateMachine->SetFirstFinishBrow(firstFinishBrowStateHash);
}

void YakuzaStateMachine::ResetAttackFlagsMachine()
{
	m_attackFlag = false;
	m_finishBrowFlag = false;
}

void YakuzaStateMachine::SetIsDefense(bool setIsKnockBack, KnockBackParam param)
{
	//ノックバックフラグ
	m_isDamageKnockBack = setIsKnockBack;
	//ノックバック内パラメーター
	m_knockBackParam = param;
}

void YakuzaStateMachine::SetIsDamage(bool setIsDamage, bool setIsKnockBack, KnockBackParam param)
{
	//ダメージフラグ
	m_isDamage = setIsDamage;
	//ノックバックフラグ
	m_isDamageKnockBack = setIsKnockBack;
	//ノックバック内パラメーター
	m_knockBackParam = param;
}

void YakuzaStateMachine::ResetIsKnockBack(const KnockBackParam& param)
{
	//ノックバックフラグを立て直す
	m_isDamageKnockBack = true;
	//ノックバック内パラメーターを立て直す
	m_knockBackParam = param;

	//アニメーションを一瞬だけ切り替える
	HasCharactarPlayAnimation(YakuzaAnimation::en_fightingIdle, 0.1f);
}

void YakuzaStateMachine::GrabStart(YakuzaCharacter* grabingYakuza)
{
	if (grabingYakuza->IsCharacterHpDead())
	{
		return;
	}

	//掴むヤツを決定
	m_isGrabing = true;
	//掴むヤツを設定
	m_grabingYakuza = grabingYakuza;
	//掴み用のコリジョンを削除
	m_hasCharactar->DeleteGrabCollision();
}

void YakuzaStateMachine::GrabEnd()
{
	//掴み終了
	m_isGrab = false;
	//掴んでいない
	m_isGrabing = false;
	//掴むヤツをNULLに
	m_grabingYakuza = nullptr;	
	//掴み用のコリジョンを削除
	m_hasCharactar->DeleteGrabCollision();
}

void YakuzaStateMachine::GrabBedStart(YakuzaCharacter* grabedYakuza)
{
	if (!grabedYakuza)
	{
		return;
	}

	//掴まれてるフラグを立てる
	m_isGrabbed = true;
	//掴まれているヤクザを設定
	m_grabBedYakuza = grabedYakuza;
}

void YakuzaStateMachine::GrabBedEnd()
{
	//掴まれ終了 
	m_isGrabbed = false;
	//掴まれているヤクザをnullに
	m_grabBedYakuza = nullptr;
}

void YakuzaStateMachine::SetHasCharactarPosition(const Vector3& pos) { m_hasCharactar->SetPosition(pos); }

const Vector3& YakuzaStateMachine::GetHasCharactarPos() { return m_hasCharactar->GetPosition(); }

void YakuzaStateMachine::SetPlayerRot(const Quaternion& rot) { m_hasCharactar->SetRotation(rot); }

Quaternion& YakuzaStateMachine::GetHasCharactarRot() { return m_hasCharactar->GetRotation(); }

void YakuzaStateMachine::SetHasCharactarForward(const Vector3& forward) { m_hasCharactar->SetForward(forward); }

Vector3& YakuzaStateMachine::GetHasCharactarForward() { return m_hasCharactar->GetForward(); }

void YakuzaStateMachine::HasCharactarPlayAnimation(int animationNum,float interpolateTime,float animationSpeed)
{
	m_hasCharactar->GetModelRender()->SetAnimationSpeed(animationSpeed);
	m_hasCharactar->GetModelRender()->PlayAnimation(animationNum, interpolateTime);
}

bool YakuzaStateMachine::IsHasCharactarPlayAnimation()
{
	return m_hasCharactar->GetModelRender()->IsPlayingAnimation();
}

bool YakuzaStateMachine::IsHasCharacterDead()
{
	return m_hasCharactar->IsDead();
}

bool YakuzaStateMachine::IsHasCharacterAttackCollisionActive()
{
	return m_hasCharactar->IsAttackCollisionActive();
}

bool YakuzaStateMachine::IsHasCharacterGrabCollisionActive()
{
	return m_hasCharactar->IsGrabCollisionActive();
}

bool YakuzaStateMachine::IsHasCharacterGrabBedEscape(bool isResistance)
{
	return YakuzaCharacterDamageManager::GetInstance()->UpdateGrabBedYakuzaEscapeTime(
		m_hasCharactar,
		isResistance
	);
}

void YakuzaStateMachine::HasCharacterKnockBackProcces(KnockBackParam& param)
{
	//y軸は無視する
	param.m_direction.y = 0.0f;

	//ノックバック方向
	Vector3 knockDir = param.m_direction;
	//ノックバック力
	float knockPower = param.m_power;


	param.m_knockElapsed += g_gameTime->GetFrameDeltaTime();

	float t = param.m_knockElapsed / param.m_duration;
	t = btClamped(t, 0.0f, 1.0f);

	//イージング処理(簡易)
	float ease = 1.0f - t;

	Vector3 moveVec = knockDir * knockPower * ease * g_gameTime->GetFrameDeltaTime();

	Vector3 newPos = GetHasCharactarCharaCon()->Execute(moveVec, 1.0f);

	//座標を設定
	SetHasCharactarPosition(newPos);

	if (t >= 1.0f)
	{
		param.m_isEndKnockBack = true;
	}
}

void YakuzaStateMachine::HasCharacterDeadProcces()
{
	m_hasCharactar->YakuzaCharacterDeadProcces();
}

void YakuzaStateMachine::HasCharacterGrabingProcces()
{
	YakuzaCharacterDamageManager::GetInstance()->UpdateBothYakuzaGrabProcess(
		m_hasCharactar,
		m_grabingYakuza
	);
}

void YakuzaStateMachine::HasCharacterSendToGrabingOrGrabBedYakuzaData(int sendDamageType)
{
	if (IsNowStateClassName<YakuzaGrabState>())
	{
		YakuzaCharacterDamageManager::GetInstance()->SendGrabingToGrabBedYakuzaData(
			m_grabingYakuza,
			sendDamageType
		);
	}
	else if (IsNowStateClassName<YakuzaGrabBedState>())
	{
		YakuzaCharacterDamageManager::GetInstance()->SendGrabBedToGrabingYakuzaData(
			m_grabBedYakuza,
			sendDamageType
		);
	}
}

void YakuzaStateMachine::HasCharacterGrabBedTakeDamage(int damageType)
{
	YakuzaCharacterDamageManager::GetInstance()->TakeGrabBedYakuzaDamage(m_hasCharactar,damageType);
}

void YakuzaStateMachine::HasCharacterToGrabBedThrownPositionUpdate()
{
	YakuzaCharacterDamageManager::GetInstance()->UpdateGrabBedYakuzaThrownPosition(
		m_hasCharactar,
		m_grabBedYakuza->GetYakuzaStateMachine().GetGrabThrowPos()
	);
}

void YakuzaStateMachine::HasCharacterGrabingYakuzaThrowPositionAdjustment(const Vector3& sweepDir, const Vector3& adjustDir,float sweepDis)
{
	YakuzaCharacterDamageManager::GetInstance()->AdjustGrabBedYakuzaPositionOnThrow(
		m_grabingYakuza,
		m_hasCharactar,
		sweepDir,
		adjustDir,
		sweepDis
	);
}

void YakuzaStateMachine::HasCharacterSetIsInvincible(bool setIsInvincible)
{
	m_hasCharactar->SetIsInvicible(setIsInvincible);
}

void YakuzaStateMachine::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"CanTransition") == 0)
	{
		m_isComboTransition = true;
	}
	if (wcscmp(eventName, L"HitBoxOn") == 0)
	{
		if (IsGetYakuzaStateMachineNowState<YakuzaAttackState>())
		{
			m_hasCharactar->SpwanAttackCollision(
				m_hasCharactar,
				20.0f
			);
		}
		else if(IsGetYakuzaStateMachineNowState<YakuzaGrabState>())
		{
			m_hasCharactar->SpawnGrabCollision(
				m_hasCharactar,
				20.0f
			);
		}
	}
	if (wcscmp(eventName, L"HitBoxOff") == 0)
	{
		if (IsGetYakuzaStateMachineNowState<YakuzaAttackState>())
		{
			m_hasCharactar->DeleteAttackCollision();
		}
	}
	//掴み時の攻撃判定イベント
	if (wcscmp(eventName, L"Attack") == 0)
	{
		SoundManager::Get().PlaySE(SoundId::se_hittingLightA, false, false, 0.5f);

		YakuzaCharacterDamageManager::GetInstance()->TakeGrabBedYakuzaDamage(m_grabingYakuza, en_grabDamage);
	}
	if (wcscmp(eventName, L"footsteps") == 0)
	{
		SoundManager::Get().PlaySE(SoundId::se_FootstepsA,false,false,0.3f);
	}
}

CharacterController* YakuzaStateMachine::GetHasCharactarCharaCon()
{
	return m_hasCharactar->GetCharacterController();
}

YakuzaAttackComboStateMachine* YakuzaStateMachine::GetAttackStateMachine()
{
	return m_attackStateMachine.get();
}

