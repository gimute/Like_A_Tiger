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

void YakuzaCharacterDamageManager::SendPlayerYakuzaDamage(float sendDamage)
{
	if (!m_playerPtr ||
		m_playerPtr->GetIsInvicible())
	{
		return;
	}
	////無敵時間開始
	//m_playerPtr->StartInvincible(3.0f);

	bool isDefense = false;

	//ガード時はダメージを半減する
	if(m_playerPtr->GetYakuzaStateMachine().
		IsGetYakuzaStateMachineNowState<YakuzaDefenseState>())
	{
		isDefense = true;

		sendDamage /= 2;
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

float YakuzaCharacterDamageManager::GetPlayerYakuzaDamage()
{
	if (!m_playerPtr)
	{
		return 0.0f;
	}

	return m_playerPtr->GetYakuzaStateMachine().GetTypeSetAttackPower();
}