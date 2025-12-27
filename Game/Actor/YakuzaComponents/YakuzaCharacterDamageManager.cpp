#include "stdafx.h"
#include "YakuzaCharacterDamageManager.h"

#include "Actor\Enemy\EnemyManager.h"
#include "Actor\Enemy\Enemy.h"

#include "Actor\YakuzaComponents\YakuzaStates.h"

//インスタンス初期化
YakuzaCharacterDamageManager* YakuzaCharacterDamageManager::m_instance = nullptr;

//void YakuzaCharacterDamageManager::SendEnemyDamage(const char* name, float sendDamage)
//{
//	auto & enemyList = EnemyManager::GetInstance()->GetEnemyPairList();
//
//	for (auto & enemyPtr : enemyList)
//	{
//		auto & bodyCollision = enemyPtr.m_enemy->GetBodyCollision();
//		
//		if (name == bodyCollision.GetName())
//		{
//			//ここにHP処理を入れる
//			int b = 0;
//		}
//	}
//}
//
//void YakuzaCharacterDamageManager::SendOtherYakuzaDamage(const char* name, float sendDamage)
//{
//	for (auto & otherPtr : m_sendDamageOtherYakuzaList)
//	{
//		auto& bodyCollision = otherPtr->GetBodyCollision();
//
//		if (name == bodyCollision.GetName())
//		{
//			int b = 0;
//		}
//	}
//}

void YakuzaCharacterDamageManager::SendPlayerYakuzaDamage(float sendDamage, const Vector3& attackerPos)
{
	if (!m_playerPtr ||
		m_playerPtr->GetIsInvicible())
	{
		return;
	}
	////無敵時間開始
	//m_playerPtr->StartInvincible(3.0f);

	bool isDefense = false;

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

			//ダメージを0に
			sendDamage = 0;
		}
	}

	m_playerPtr->TakePlayerHp(sendDamage);

	//ここにダメージ処理
	if (m_playerPtr->IsCharacterHpDead())
	{
		m_playerPtr->GetYakuzaStateMachine().SetIsDead(true);
	}
	else
	{
		if (!isDefense)
		{
			m_playerPtr->GetYakuzaStateMachine().SetIsDamage(true);
		}
	}	
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

float YakuzaCharacterDamageManager::GetPlayerYakuzaDamage()
{
	if (!m_playerPtr)
	{
		return 0.0f;
	}

	return m_playerPtr->GetYakuzaStateMachine().GetTypeSetAttackPower();
}