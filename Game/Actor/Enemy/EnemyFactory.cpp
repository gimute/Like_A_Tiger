#include "stdafx.h"
#include "EnemyFactory.h"
#include "EnemyTypeSet\EnemyTypeSetFactory.h"

#include "Actor\Enemy\Enemy.h"

Character* EnemyFactory::CreateEnemy(EnemyType type)
{

	Enemy* newEnemy = NewGO<Enemy>(0,"enemy");

	//TypeSet取得
	auto typeSet = EnemyTypeSetFactory::GetInstance().Create(type);
	//TypeSet攻撃ステートリスト作成
	auto typeSetAttackList = typeSet.get()->CreateActions(newEnemy->GetYakuzaStateMachine()->GetAttackStateMachine());
	//攻撃ステート登録
	for (auto it = typeSetAttackList.begin(); it != typeSetAttackList.end();)
	{
		uint32_t id = it->first;

		newEnemy->GetYakuzaStateMachine()->GetAttackStateMachine()->AddState(
			id,
			std::move(it->second.get())
		);
	}


	return nullptr;
}

