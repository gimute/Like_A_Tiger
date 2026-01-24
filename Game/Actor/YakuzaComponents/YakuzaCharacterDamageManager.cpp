#include "stdafx.h"
#include "YakuzaCharacterDamageManager.h"

#include "Actor\Enemy\EnemyManager.h"
#include "Actor\Enemy\Enemy.h"

#include "Actor\YakuzaComponents\YakuzaStates.h"

#include "Sound\SoundManager.h"

//インスタンス初期化
YakuzaCharacterDamageManager* YakuzaCharacterDamageManager::m_instance = nullptr;

namespace YCDM_Constant
{
	const float GRABBED_CHARACTER_POS_ADD = 30.0f;
}

void YakuzaCharacterDamageManager::SendEnemyYakuzaDamage(Enemy* sendEnemy, YakuzaDamageDatas sendDamage)
{
	if (!sendEnemy ||
		sendEnemy->GetIsInvicible())
	{
		return;
	}

	Vector3 sendEnemyPos = sendEnemy->GetPosition();

	bool isDefense = false;
	bool isKnockBack = false;

	KnockBackParam param;

	//ガード時はダメージを0にする
	if (m_playerPtr->GetYakuzaStateMachine().
		IsGetYakuzaStateMachineNowState<YakuzaDefenseState>())
	{
		//角度によって防御成功判定
		if (IsDefenseSuccessful(
			sendEnemyPos,
			sendEnemy->GetForward(),
			m_playerPtr->GetPosition(),
			0.3f//前側約140度は防御成功扱い
		))
		{
			isDefense = true;
		}
	}

	Vector3 distNomal = sendEnemyPos - m_playerPtr->GetPosition();
	distNomal.Normalize();

	param = KnockBackParam(
		distNomal,
		sendDamage.m_knockBackPow,
		0.3f
	);

	if (!isDefense)
	{
		sendEnemy->TakeDamage(sendDamage.m_attackPow);
	}

	//ここにダメージ処理
	if (sendEnemy->IsCharacterHpDead())
	{
		sendEnemy->GetYakuzaStateMachine().ResetIsKnockBack(param);

		sendEnemy->GetYakuzaStateMachine().SetIsDead(true);
	}
	else
	{
		if (!isDefense)
		{
			if (sendEnemy->GetYakuzaStateMachine().GetIsDamage())
			{
				sendEnemy->GetYakuzaStateMachine().ResetIsKnockBack(param);
			}
			else
			{
				sendEnemy->GetYakuzaStateMachine().SetIsDamage(true, true, param);
			}
		}
		else
		{
			sendEnemy->GetYakuzaStateMachine().SetIsDefense(true, param);
			
			SoundManager::Get().PlaySE(SoundId::se_hittingDefenseA);

			return;
		}
	}

	SoundManager::Get().PlaySE(sendDamage.m_seId);
}

void YakuzaCharacterDamageManager::SendPlayerYakuzaDamage(YakuzaDamageDatas sendDamage, const Vector3& attackerPos)
{
	if (!m_playerPtr ||
		m_playerPtr->GetIsInvicible())
	{
		return;
	}
	////無敵時間開始
	//m_playerPtr->StartInvincible(3.0f);

	bool isDefense = false;

	KnockBackParam param;

	//ガード時はダメージを0にする
	if(m_playerPtr->GetYakuzaStateMachine().
		IsGetYakuzaStateMachineNowState<YakuzaDefenseState>())
	{
		//角度によって防御成功判定
		if (IsDefenseSuccessful(
			m_playerPtr->GetPosition(),
			m_playerPtr->GetForward(),
			attackerPos,
			0.3f//前側約140度は防御成功扱い
		))
		{
			isDefense = true;
		}
	}

	Vector3 distNomal = m_playerPtr->GetPosition() - attackerPos;
	distNomal.Normalize();

	param = KnockBackParam(
		distNomal,
		sendDamage.m_knockBackPow,
		0.3f
	);

	if (!isDefense)
	{
		m_playerPtr->TakePlayerHp(sendDamage.m_attackPow);
	}

	//ここにダメージ処理
	if (m_playerPtr->IsCharacterHpDead())
	{
		m_playerPtr->GetYakuzaStateMachine().ResetIsKnockBack(param);

		m_playerPtr->GetYakuzaStateMachine().SetIsDead(true);
	}
	else
	{
		if (!isDefense)
		{
			if (m_playerPtr->GetYakuzaStateMachine().GetIsDamage())
			{
				m_playerPtr->GetYakuzaStateMachine().ResetIsKnockBack(param);
			}
			else
			{
				m_playerPtr->GetYakuzaStateMachine().SetIsDamage(true, true, param);
			}
		}
		else
		{
			m_playerPtr->GetYakuzaStateMachine().SetIsDefense(true, param);

			SoundManager::Get().PlaySE(SoundId::se_hittingDefenseA);

			return;
		}
	}	

	SoundManager::Get().PlaySE(sendDamage.m_seId);
}

YakuzaCharacter* YakuzaCharacterDamageManager::SendPlayerGrabEnemyYakuza(YakuzaCharacter* grabYakuza)
{
	m_playerPtr->GetYakuzaStateMachine().GrabStart(grabYakuza);

	return m_playerPtr;
}

YakuzaCharacter* YakuzaCharacterDamageManager::SendEnemyGrabPlayerYakuza(YakuzaCharacter* grabYakuza)
{
	m_playerPtr->GetYakuzaStateMachine().GrabBedStart(grabYakuza);

	return m_playerPtr;
}



bool YakuzaCharacterDamageManager::UpdateBothYakuzaGrabProcess(YakuzaCharacter* grabingYakuza, YakuzaCharacter* grabBedYakuza)
{
	//掴まれている側が死亡してしまったら処理を継続できないためflase
	if (grabBedYakuza->IsCharacterHpDead())
	{
		return false;
	}
	Vector3 GBIYakuzaPos = grabingYakuza->GetPosition(); //掴んでいる側の位置
	Vector3 GBIYakuzaFor = grabingYakuza->GetForward(); //掴んでいる側の正面ベクトル
	Vector3 GBBYakuzaPos = grabBedYakuza->GetPosition(); //掴まれている側の位置
	Vector3 GBBYakuzaFor = grabBedYakuza->GetForward(); //掴まれている側の正面ベクトル
	
	//掴んでいる側の処理(今は移動とかはないため無し)
	//掴まれている側の処理
	//掴まれている側の更新位置
	Vector3 grabBedPos = GBIYakuzaPos + (GBIYakuzaFor * YCDM_Constant::GRABBED_CHARACTER_POS_ADD);

	grabBedYakuza->SetPosition(grabBedPos);
}

bool YakuzaCharacterDamageManager::IsDefenseSuccessful(
	const Vector3& defenderPos,
	const Vector3& defenderForward,
	const Vector3& attackerPos,
	float defenseAngleCos
)
{
	//防御者から攻撃者へのベクトルを求める
	Vector3 toAttacker = attackerPos - defenderPos;
	toAttacker.Normalize();

	//正面方向も正規化
	Vector3 foward = defenderForward;
	foward.Normalize();

	//内積を求める
	float dot = foward.Dot(toAttacker);

	//正面側なら秒魚成功
	if (dot >= defenseAngleCos)
	{
		return true;
	}

	return false;
}

YakuzaDamageDatas YakuzaCharacterDamageManager::GetPlayerYakuzaDamage()
{
	if (!m_playerPtr)
	{
		YakuzaDamageDatas nullData;

		return nullData;
	}

	return m_playerPtr->GetYakuzaStateMachine().GetTypeSetAttackPower();
}