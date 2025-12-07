#include "stdafx.h"
#include "YakuzaCharacterDamageManager.h"

#include "Actor\Enemy\EnemyManager.h"
#include "Actor\Enemy\Enemy.h"

//インスタンス初期化
YakuzaCharacterDamageManager* YakuzaCharacterDamageManager::m_instance = nullptr;

void YakuzaCharacterDamageManager::SendEnemyDamage(CollisionObject* collision, float sendDamage)
{
	auto & enemyList = EnemyManager::GetInstance()->GetEnemyPairList();

	for (auto & enemyPtr : enemyList)
	{
		auto & bodyCollision = enemyPtr.m_enemy->GetBodyCollision();
		
		if (collision == &bodyCollision)
		{
			//ここにHP処理を入れる
		}
	}
}